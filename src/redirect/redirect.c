/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirect.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/04 20:47:03 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/04 20:47:03 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <redir.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define DEST	0
#define FLAG	1
#define MODE	2
#define CLOSE_ERR	"Something went wrong trying to close a previous redir:"
#define OPEN_ERR	"Something went wrong trying to open a file for redir:"
#define UNLINK_ERR	"Unlinking heredoc failed? This should never happen?"

static const int32_t	g_redir_opts[][3] = {
[RED_IN - RED_IN] = {STDIN_FILENO, O_RDONLY, 0},
[RED_HD - RED_IN] = {STDIN_FILENO, O_RDONLY, 0},
[RED_HD_Q - RED_IN] = {STDIN_FILENO, O_RDONLY, 0},
[RED_OUT - RED_IN] = {STDOUT_FILENO, O_TRUNC | O_CREAT | O_WRONLY, 0644},
[RED_APP - RED_IN] = {STDOUT_FILENO, O_APPEND | O_CREAT | O_WRONLY, 0644}
};

static bool	error(const char *where)
{
	perror(where);
	return (false);
}

bool	redirect(t_redir *redir, int32_t fds[2])
{
	const int32_t	*opts;

	opts = g_redir_opts[redir->type - RED_IN];
	if (fds[opts[DEST]] != opts[DEST] && fds[opts[DEST]] != -1)
		if (close(fds[opts[DEST]]) == -1)
			error(CLOSE_ERR);
	fds[opts[DEST]] = open(redir->str, opts[FLAG], opts[MODE]);
	if (fds[opts[DEST]] == -1)
		return (error(OPEN_ERR));
	if (redir->type == RED_HD || redir->type == RED_HD_Q)
		if (unlink(redir->str) == -1)
			error(UNLINK_ERR);
	return (true);
}
