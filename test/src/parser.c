/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/20 20:16:06 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/20 20:16:06 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>
#include <get_next_line.h>
#include <unistd.h>
#include <stdio.h>
#include <minishell.h>
#include <redir.h>
#include <environ.h>

static const char	*g_ast_typenames[] = {
[COMMAND] = "COMMAND",
[LOGIC] = "LOGIC",
[PARENTHESIS] = "PARENTHESIS",
[PIPELINE] = "PIPELINE",
};

static const char	*g_red_typenames[] = {
[RED_IN] = "< %s ",
[RED_HD] = "<< %s ",
[RED_HD_Q] = "<< \"%s\" ",
[RED_APP] = ">> %s ",
[RED_OUT] = "> %s "
};

t_sh_env	g_globals = {.exit = 0};

static void	indent(size_t depth)
{
	while (depth--)
		printf("  ");
}

static void	print_cmd(t_cmd_node *node, size_t depth)
{
	size_t	i;
	t_redir	*r;

	indent(depth + 1);
	printf("argv: ");
	i = 0;
	while (i < node->argv.length)
		printf("\"%s\" ", *((char **) dynarr_get(&node->argv, i++)));
	printf("\n");
	indent(depth + 1);
	printf("redir: ");
	i = 0;
	while (i < node->redirs.length)
	{
		r = dynarr_get(&node->redirs, i++);
		printf(g_red_typenames[r->type], r->str);
	}
	printf("\n");
}

static void	print_node(t_ast_node *node, size_t depth)
{
	size_t	i;

	indent(depth);
	printf("%s\n", g_ast_typenames[node->type]);
	if (node->type == COMMAND)
		print_cmd(&node->node.command, depth);
	else if (node->type == PARENTHESIS)
		print_node(node->node.paren.contents, depth + 1);
	else if (node->type == LOGIC)
	{
		indent(depth + 1);
		if (node->node.logic.type == OR)
			printf("OR\n");
		else
			printf("AND\n");
		indent(depth + 1);
		printf("Left:\n");
		print_node(node->node.logic.l, depth + 2);
		indent(depth + 1);
		printf("Right:\n");
		print_node(node->node.logic.r, depth + 2);
	}
	else
	{
		i = 0;
		while (i < node->node.pipe.nodes.length)
			print_node((*(t_ast_node **) dynarr_get(&node->node.pipe.nodes, i++)), depth + 1);
	}
	indent(depth);
	printf("%s END\n", g_ast_typenames[node->type]);
	if (depth == 0)
		printf("\n");
}

static void	test(char *line)
{
	t_dynarr	exp_tokens;
	t_ast_node	*root = NULL;
	size_t		i;

	if (parse_input(line, &exp_tokens, &root))
		return ((void) printf("Parsing failed for %s\n", line));
	print_node(root, 0);
	destroy_node(&root);
	i = 0;
	while (i < exp_tokens.length)
		free(((t_exp_tok *) exp_tokens.arr)[i++].str);
	dynarr_delete(&exp_tokens);
}

int	main(void)
{
	char	*line;

	init_environment();
	while (true)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break ;
		line[ft_strlen(line) - 1] = '\0';
		if (*line == '%')
			set_variable(line, var_name_end(line));
		else if (*line != '#')
			test(line);
		else
			printf("%s\n", line + 1);
		free(line);
	}
	clean_environment();
}
