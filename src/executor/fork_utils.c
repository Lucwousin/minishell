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

#include <stdint.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

uint8_t	wait_for(pid_t pid)
{
	int32_t	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (0x80 + WTERMSIG(status));
	return (0);
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
		perror("Forking failed");
	}
	return (true);
}
