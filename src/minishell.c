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

#include <minishell.h>
#include <environ.h>
#include <execute.h>
#include <parse.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

bool general_error(const char *where);

uint8_t	parse_input(const char *input, t_dynarr *ex_toks, t_ast_node **dst)
{
	uint8_t		status;
	t_dynarr	tokens;

	status = tokenize(&tokens, input);
	if (status != EXIT_SUCCESS && !general_error("tokenize"))
		return (status);
	status = evaluate(&tokens);
	if (status != EXIT_SUCCESS && !general_error("evaluate"))
		return (status);
	status = preparse(input, &tokens, ex_toks);
	if (status != EXIT_SUCCESS)
		return (status);
	return (build_ast(ex_toks, dst));
}

static uint8_t	handle_input(const char *input)
{
	uint8_t		status;
	t_dynarr	ex_toks;
	t_ast_node	*task;

	task = NULL;
	status = parse_input(input, &ex_toks, &task);
	if (status != EXIT_SUCCESS || task == NULL)
		return (status);
	status = execute(task);
	destroy_node(&task);
	for (size_t i = 0; i < ex_toks.length; ++i)
		free(((t_exp_tok *) ex_toks.arr)[i].str);
	dynarr_delete(&ex_toks);
	return (status);
}

static void ign()
{
}

uint8_t	minishell(int argc, char **argv)
{
	char	*input;

	if (init_environment() == EXIT_FAILURE)
		return (EXIT_FAILURE);
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
		handle_input(input);
		free(input);
	}
	clean_environment();
	return (EXIT_SUCCESS);
}