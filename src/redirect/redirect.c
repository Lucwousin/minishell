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
#define CLOSE_ERR	"Something went wrong trying to close a file"
#define OPEN_ERR	"Something went wrong trying to open a file"
#define UNLINK_ERR	"Unlinking heredoc failed? This should never happen?"
#define STDIN_ERR	"Redirecting to stdin failed"
#define STDOUT_ERR	"Redirecting to stdout failed"

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
	if (fds[opts[DEST]] != -1)
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

bool	dup_stdio(int32_t dst[2])
{
	dst[STDIN_FILENO] = dup(STDIN_FILENO);
	dst[STDOUT_FILENO] = dup(STDOUT_FILENO);
	if (dst[STDIN_FILENO] != -1 && dst[STDOUT_FILENO] != -1)
		return (true);
	perror("duplicating original stdin/out failed");
	if (dst[STDIN_FILENO] != -1)
		close(dst[STDIN_FILENO]);
	if (dst[STDOUT_FILENO] != -1)
		close(dst[STDOUT_FILENO]);
	return (false);
}

bool	redir_stdio(int32_t fds[2])
{
	static const char	*err[2] = {STDIN_ERR, STDOUT_ERR};
	uint8_t				d;

	d = STDERR_FILENO;
	while (d-- > STDIN_FILENO)
	{
		if (fds[d] < 0)
			continue ;
		if (dup2(fds[d], d) < 0)
			perror(err[d]);
		if (close(fds[d]) != 0)
			perror(err[d]);
		else
			fds[d] = -1;
	}
	if (fds[0] == -1 && fds[1] == -1)
		return (true);
	return (false);
}
