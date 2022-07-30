/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 18:29:19 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/30 18:29:19 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <dynarr.h>
#include <minishell.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <parse.h>

bool			malloc_error(const char *where);

static const char	*g_ast_typenames[] = {
		[COMMAND] = "COMMAND",
		[LOGICAL_EXPRESSION] = "LOGICAL_EXPRESSION",
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
	else if (node->type == LOGICAL_EXPRESSION)
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

static void	do_something_with_input(const char *input)
{
	t_dynarr	tokens;
	t_dynarr	expanded_tokens;
	t_ast_node	*task;
	
	if (!tokenize(&tokens, input))
		return ((void) malloc_error("tokenize"));
	if (!evaluate(&tokens))
		return (malloc_error("evaluate"), dynarr_delete(&tokens));
	if (!preparse(input, &tokens, &expanded_tokens, -69))
		return (dynarr_delete(&tokens));
	dynarr_delete(&tokens);
	task = build_ast(&expanded_tokens);
	if (task != NULL)
	{
		print_node(task, 0);
		destroy_node(&task);
	}
	size_t i = 0;
	while (i < expanded_tokens.length)
		free(((t_exp_tok *) expanded_tokens.arr)[i++].str);
	dynarr_delete(&expanded_tokens);
	//expanded = expand(&tokens, input);
	//for (int i = 0; expanded[i].type != END_OF_INPUT; i++)
	//	printf("%s\n", expanded[i].str);

	// Lexer
	// Parser
	// Executor
	// This will probably be fed a struct used for ~~everything~~?
	// At least the env params, a pointer we can put the exit code
	// Probably dup'd file descriptors for stdin, stdout and stderr (do we need to handle 2> or &>?)
	// I still think the pid's of child processes should probably be the global, so we can kill them
	// if we receive Ctrl+C
	// We probably need bool in the struct for whether or not we need to exit as well (or we can just clean up in the function handling that, probably)

}

void	minishell(int argc, char **argv, char **envp)
{
	char	*input;

	(void) argc;
	(void) argv;
	(void) envp; // We are going to need this in the executor (execve needs it as param)
	// Init state struct, dup stdin stdout stderr?
	// Init signal handlers
	while (true)
	{
		input = readline(PROMPT);
		if (!input)
			break ;
		if	(*input)
			add_history(input);
		do_something_with_input(input);
		free(input);
	}
}