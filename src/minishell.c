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
#include <signal.h>
#include "execute.h"

bool			malloc_error(const char *where);

static void	do_something_with_input(const char *input)
{
	t_dynarr	tokens;
	t_dynarr	expanded_tokens;
	t_ast_node	*task;

	if (!tokenize(&tokens, input))
		return ((void) malloc_error("tokenize"));
	if (!evaluate(&tokens))
		return (malloc_error("evaluate"), dynarr_delete(&tokens));
	if (!preparse(input, &tokens, &expanded_tokens))
		return (dynarr_delete(&tokens));
	dynarr_delete(&tokens);
	task = build_ast(&expanded_tokens);
	if (task != NULL)
	{
		execute(task);
		destroy_node(&task);
	}
	size_t i = 0;
	while (i < expanded_tokens.length)
		free(((t_exp_tok *) expanded_tokens.arr)[i++].str);
	dynarr_delete(&expanded_tokens);
}

static void ign()
{
}

void	minishell(int argc, char **argv)
{
	char	*input;

	signal(SIGINT, ign);
	(void) argc;
	(void) argv;
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