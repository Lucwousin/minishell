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

uint8_t	wait_for(pid_t pid, uint8_t *exit)
{
	int32_t	status_loc;

	if (signal_ignore_interrupt() != 0)
		return (EXIT_FAILURE);
	if (waitpid(pid, &status_loc, 0) == -1)
		return (perror("waitpid"), EXIT_FAILURE);
	if (WIFEXITED(status_loc))
		*exit = WEXITSTATUS(status_loc);
	if (WIFSIGNALED(status_loc))
		*exit = 0x80 + WTERMSIG(status_loc);
	if (*exit == 0x80 + SIGINT)
		g_env.interrupted = true;
	if (signal_standard_interrupt() != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

bool	fork_and_wait(uint8_t *status)
{
	pid_t	pid;

	if (!do_fork(&pid))
	{
		*status = EXIT_FAILURE;
		g_env.exit = EXIT_FAILURE;
		return (true);
	}
	if (pid == 0)
		return (false);
	*status = wait_for(pid, &g_env.exit);
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
			status = wait_for(pids[i], &g_env.exit);
		if (status != EXIT_SUCCESS && g_env.exit == EXIT_SUCCESS)
			g_env.exit = status;
		++i;
	}
	return (status);
}

bool	do_fork(pid_t *pid_p)
{
	*pid_p = fork();
	if (*pid_p < 0)
	{
		perror("forking_failed");
		return (false);
	}
	return (true);
}
