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
#include <stdlib.h>
#include <stdio.h>

static void	standard_interrupt(int32_t sig)
{
	g_globals.interrupted = true;
	g_globals.exit = 0x80 + sig;
}

uint8_t	signal_standard_interrupt(void)
{
	const struct sigaction	action = (struct sigaction){\
		{standard_interrupt}, 0, 0
	};

	if (sigaction(SIGINT, &action, NULL) == 0)
		return (EXIT_SUCCESS);
	perror("signal_standard_interrupt");
	return (EXIT_FAILURE);
}

uint8_t	signal_ignore_interrupt(void)
{
	if (signal(SIGINT, SIG_IGN) != SIG_ERR)
		return (EXIT_SUCCESS);
	perror("signal_ignore_interrupt");
	return (EXIT_FAILURE);
}

uint8_t	init_signals(void)
{
	if (sigaction(SIGINT, NULL, g_globals.orig_signals + 0) == 0 && \
		sigaction(SIGQUIT, NULL, g_globals.orig_signals + 1) == 0 && \
		signal_standard_interrupt() != EXIT_FAILURE && \
		signal(SIGQUIT, SIG_IGN) != SIG_ERR)
		return (EXIT_SUCCESS);
	perror("init_signals");
	return (EXIT_FAILURE);
}

uint8_t	reset_signals(void)
{
	if (sigaction(SIGINT, g_globals.orig_signals + 0, NULL) >= 0 && \
		sigaction(SIGQUIT, g_globals.orig_signals + 1, NULL) >= 0)
		return (EXIT_SUCCESS);
	perror("reset_signals");
	return (EXIT_FAILURE);
}
