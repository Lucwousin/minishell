/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_command.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/27 14:44:02 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/27 14:44:02 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>
#include <execute.h>
#include <redir.h>
#include <stdlib.h>
#include <unistd.h>

static bool	do_redirs(t_redir *redirs, size_t len, int32_t fds[2], bool builtin)
{
	size_t	i;

	fds[0] = STDIN_FILENO;
	fds[1] = STDOUT_FILENO;
	i = 0;
	while (i < len)
	{
		if (!redirect(redirs + i++, fds) && builtin)
		{
			if (fds[0] != -1 && fds[0] != STDIN_FILENO)
				close(fds[0]);
			if (fds[1] != -1 && fds[1] != STDOUT_FILENO)
				close(fds[0]);
			return (false);
		}
	}
	if (fds[0] != -1 && dup2(fds[0], STDIN_FILENO) == -1 && builtin)
		return (false);
	if (fds[1] != -1 && dup2(fds[1], STDOUT_FILENO) == -1 && builtin)
		return (false);
	return (true);
}

uint8_t	execute_command(t_cmd_node *cmd, bool can_exit)
{
	t_builtin	builtin;
	int32_t		fds[2];
	uint8_t		status;

	builtin = identify_command(cmd->argv.arr);
	if (builtin == NONE && !can_exit)
	{
		if (fork_and_wait(&status))
			return (status);
		can_exit = true;
	}
	if (!do_redirs(cmd->redirs.arr, cmd->redirs.length, fds, builtin != NONE))
		return (finish_leaf(EXIT_FAILURE, can_exit));
	if (builtin != NONE)
		return (finish_leaf(execute_builtin(builtin, cmd), can_exit));
	execute_binary(cmd);
	return (69);
}
