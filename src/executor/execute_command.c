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
#include <redir.h>
#include <unistd.h>

static bool	do_redirs(t_dynarr *redirs, int32_t orig[2], bool builtin)
{
	size_t	i;
	int32_t	fds[2];

	if (builtin && !dup_stdio(orig))
		return (false);
	fds[0] = -1;
	fds[1] = -1;
	i = 0;
	while (i < redirs->length)
	{
		if (redirect(((t_redir *) redirs->arr) + i++, fds) || !builtin)
			continue;
		if (fds[0] != -1)
			close(fds[0]);
		if (fds[1] != -1)
			close(fds[0]);
		return (false);
	}
	if (redir_stdio(fds) || !builtin)
		return (true);
	redir_stdio(orig);
	return (false);
}

uint8_t	execute_command(t_cmd_node *cmd, bool must_exit)
{
	t_builtin	builtin;
	uint8_t		status;
	int32_t		orig[2];

	builtin = identify_command(cmd->argv.arr);
	if (builtin == NONE && !must_exit)
	{
		if (fork_and_wait(&status))
			return (status);
		must_exit = true;
	}
	if (cmd->redirs.length > 0)
		if (!do_redirs(&cmd->redirs, orig, builtin != NONE))
			return (try_exit(EXIT_FAILURE, must_exit));
	if (builtin == NONE)
		execute_binary(cmd);
	status = execute_builtin(builtin, cmd);
	if (cmd->redirs.length > 0)
		redir_stdio(orig);
	return (try_exit(status, must_exit));
}
