/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucas <lucas@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/20 20:16:06 by lucas         #+#    #+#                 */
/*   Updated: 2022/07/20 20:16:06 by lucas         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>
#include <get_next_line.h>
#include <unistd.h>
#include <stdio.h>

static const char	*g_ast_typenames[] = {
[COMMAND] = "COMMAND",
[LOGICAL_EXPRESSION] = "LOGICAL_EXPRESSION",
[PARENTHESIS] = "PARENTHESIS",
[PIPELINE] = "PIPELINE",
};

static void	set_var(char *line)
{
	static const char	*save[100];
	static int			save_idx;

	if (line == NULL)
	{
		while (save_idx--)
			free((char *) save[save_idx]);
		return ;
	}
	line[ft_strlen(line) - 1] = '\0';
	putenv(line);
	save[save_idx++] = line;
}

static void	print_cmd(t_cmd_node *node, size_t depth)
{
	size_t	i;

	i = 0;
	while (i++ < depth)
		printf("  ");
	printf("argv: ");
	i = 0;
	while (i < node->argv.length)
		printf("\"%s\" ", *((char **) dynarr_get(&node->argv, i++)));
	printf("\n");
	// TODO: Print redirections, test cases with redirections
}


static void	print_node(t_ast_node *node, size_t depth)
{
	size_t	i;

	i = 0;
	while (i++ < depth)
		printf("  ");
	printf("Type = %s\n", g_ast_typenames[node->type]);
	if (node->type == COMMAND)
		print_cmd(&node->node.command, depth);
	else if (node->type == PARENTHESIS)
		print_node(node->node.paren.contents, depth + 1);
	else if (node->type == LOGICAL_EXPRESSION)
	{
		i = 0;
		while (i++ < depth)
			printf("  ");
		if (node->node.logic.type == OR)
			printf("type OR\n");
		else
			printf("type AND\n");
		print_node(node->node.logic.l, depth + 1);
		print_node(node->node.logic.r, depth + 1);
	}
	else
	{
		i = 0;
		while (i < node->node.pipe.nodes.length)
			print_node(*((t_ast_node **) dynarr_get(&node->node.pipe.nodes, i++)), depth + 1);
	}
	i = 0;
	while (i++ < depth)
		printf("  ");
	printf("End %s\n", g_ast_typenames[node->type]);
	if (depth == 0)
		printf("\n");
}

static void	test(char *line)
{
	t_dynarr	tokens;
	t_dynarr	exp_tokens;
	t_ast_node	*root;
	size_t		i;

	i = ft_strlen(line);
	if (line[i - 1] == '\n')
		line[i - 1] = '\0';
	tokenize(&tokens, line);
	evaluate(&tokens);
	preparse(line, &tokens, &exp_tokens, 0);
	root = build_ast(&exp_tokens);
	print_node(root, 0);
	destroy_node(&root);
	i = 0;
	while (i < exp_tokens.length)
		free(((t_exp_tok *) exp_tokens.arr)[i++].str);
	dynarr_delete(&exp_tokens);
	dynarr_delete(&tokens);
}

int	main(void)
{
	char	*line;

	while (true)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break ;
		if (*line == '%')
			set_var(ft_strdup(line + 1));
		else if (*line != '#')
			test(line);
		else
			printf("%s", line + 1);
		free(line);
	}
	set_var(NULL);
}
