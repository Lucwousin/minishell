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
#include <signals.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

bool	general_error(const char *where);

uint8_t	minishell(int argc, char **argv)
{
	char	*input;

	if (init_environment() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	(void) argc;
	(void) argv;
	while (true)
	{
		if (signal_readline())
			return (EXIT_FAILURE);
		input = readline(PROMPT);
		if (!input)
			break ;
		if (signal_catch_int())
			return (EXIT_FAILURE);
		if (*input)
			handle_input(input);
		free(input);
	}
	printf("exit\n");
	clean_environment();
	rl_clear_history();
	return (g_globals.exit);
}
