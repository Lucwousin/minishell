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
#include <signals.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <libft.h>

#define FLAG_C	"-c"

bool	general_error(const char *where);

static uint8_t	init_minishell(void)
{
	if (init_signals() != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (init_environment() != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static bool	check_flag(int argc, char **argv)
{
	if (argc < 2)
		return (false);
	return (ft_strncmp(FLAG_C, argv[1], 3) == 0);
}

static uint8_t	get_line(char **dst)
{
	if (signal_readline_interrupt() != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	*dst = readline(PROMPT);
	if (signal_standard_interrupt() == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	free(*dst);
	return (EXIT_FAILURE);
}

static uint8_t	input_readline(void)
{
	char	*input;
	uint8_t	status;

	using_history();
	while (true)
	{
		input = NULL;
		status = get_line(&input);
		if (!input || status != EXIT_SUCCESS)
			break ;
		if (*input)
		{
			add_history(input);
			handle_input(input);
		}
		free(input);
	}
	printf("exit\n");
	rl_clear_history();
	return (status);
}

uint8_t	minishell(int argc, char **argv)
{
	const bool	flag_c = check_flag(argc, argv);

	if (init_minishell() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (flag_c && argc > 2)
		handle_input(argv[2]);
	else if (input_readline() == EXIT_FAILURE)
		if (g_env.exit == EXIT_SUCCESS)
			g_env.exit = EXIT_FAILURE;
	clean_environment();
	return (g_env.exit);
}
