/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/10 18:29:19 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/10 18:29:19 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>

static void	rl_interrupt(int32_t sig)
{
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
	g_globals.exit = 0x80 + sig;
}

uint8_t	signal_readline(void)
{
	if (signal(SIGINT, rl_interrupt) == SIG_ERR)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static void	default_interrupt(int32_t sig)
{
	g_globals.exit = 0x80 + sig;
}

uint8_t	signal_catch_int(void)
{
	if (signal(SIGINT, default_interrupt) == SIG_ERR)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
