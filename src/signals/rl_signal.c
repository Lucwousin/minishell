/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   rl_signal.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/11 18:29:45 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/11 18:29:45 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>

static void	rl_interrupt(int32_t sig)
{
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
	g_env.exit = 0x80 + sig;
}

uint8_t	signal_readline_interrupt(void)
{
	if (signal(SIGINT, rl_interrupt) != SIG_ERR)
		return (EXIT_SUCCESS);
	perror("signal_readline_interrupt");
	return (EXIT_FAILURE);
}
