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
#include <execute.h>
#include <libft.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#define DEST	0
#define FLAG	1
#define MODE	2
#define CLOSE_ERR	"closing file failed"
#define STDIN_ERR	"redirecting to stdin failed"
#define STDOUT_ERR	"redirecting to stdout failed"

static const int32_t	g_redir_opts[][3] = {
[RED_IN - RED_IN] = {STDIN_FILENO, O_RDONLY, 0},
[RED_HD - RED_IN] = {STDIN_FILENO, O_RDONLY, 0},
[RED_HD_Q - RED_IN] = {STDIN_FILENO, O_RDONLY, 0},
[RED_OUT - RED_IN] = {STDOUT_FILENO, O_TRUNC | O_CREAT | O_WRONLY, 0644},
[RED_APP - RED_IN] = {STDOUT_FILENO, O_APPEND | O_CREAT | O_WRONLY, 0644}
};

static bool	error(const char *file)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(file);
	return (false);
}

static const char	*expand_redir(t_redir *redir)
{
	t_dynarr	buf;

	if (redir->type == RED_HD || redir->type == RED_HD_Q)
	{
		if (!write_heredoc(redir))
			return (NULL);
		return (redir->hd.file);
	}
	dynarr_create(&buf, NAME_MAX, sizeof(char));
	if (!expand_variables(&redir->wl, &buf) || \
		!split_words(&redir->wl) || \
		!remove_quotes(&redir->wl, &buf) || \
		!expand_filenames(&redir->wl))
	{
		dynarr_delete(&buf);
		return (NULL);
	}
	dynarr_delete(&buf);
	if (redir->wl.next == NULL || redir->wl.next->next != NULL)
	{
		ft_putendl_fd("ambiguous redirect", STDERR_FILENO);
		return (NULL);
	}
	return (redir->wl.next->word);
}

bool	redirect(t_redir *redir, int32_t fds[2])
{
	const bool		is_here = redir->type == RED_HD || redir->type == RED_HD_Q;
	const int32_t	*opts = g_redir_opts[redir->type - RED_IN];
	const char		*file = expand_redir(redir);

	if (file == NULL)
		return (false);
	if (fds[opts[DEST]] != -1)
		if (close(fds[opts[DEST]]) == -1)
			error(CLOSE_ERR);
	fds[opts[DEST]] = open(file, opts[FLAG], opts[MODE]);
	if (fds[opts[DEST]] == -1)
		return (error(file));
	else if (is_here && unlink(redir->hd.file) == -1)
		error(redir->hd.file);
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
