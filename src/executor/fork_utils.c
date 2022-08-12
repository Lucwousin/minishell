/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fork_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/05 19:32:37 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/05 19:32:37 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <execute.h>
#include <signals.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

uint8_t	wait_for(pid_t pid)
{
	int32_t	status_loc;

	if (signal_ignore_interrupt() != 0)
		return (EXIT_FAILURE);
	if (waitpid(pid, &status_loc, 0) == -1)
		return (perror("waitpid"), EXIT_FAILURE);
	if (WIFEXITED(status_loc))
		g_globals.exit = WEXITSTATUS(status_loc);
	if (WIFSIGNALED(status_loc))
		g_globals.exit = 0x80 + WTERMSIG(status_loc);
	if (g_globals.exit == 0x80 + SIGINT)
		g_globals.interrupted = true;
	if (signal_standard_interrupt() != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

bool	fork_and_wait(uint8_t *status)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		return (false);
	if (pid > 0)
		*status = wait_for(pid);
	else
	{
		*status = EXIT_FAILURE;
		g_globals.exit = EXIT_FAILURE;
		perror("Forking failed");
	}
	return (true);
}

uint8_t	wait_pids(pid_t *pids, size_t len)
{
	uint8_t	status;
	size_t	i;

	i = 0;
	status = EXIT_SUCCESS;
	while (i < len)
	{
		if (pids[i] == -1)
			status = EXIT_FAILURE;
		else
			status = wait_for(pids[i]);
		if (status != EXIT_SUCCESS && g_globals.exit == EXIT_SUCCESS)
			g_globals.exit = status;
		++i;
	}
	return (status);
}
