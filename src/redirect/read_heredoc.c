/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   read_heredoc.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/04 16:15:54 by lsinke        #+#    #+#                 */
/*   Updated: 2022/09/11 19:02:26 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>
#include <redir.h>
#include <get_next_line.h>
#include <minishell.h>
#include <unistd.h>
#include <stdio.h>

#define WARN_EOF	"bash: warning: here-document delimited by end-of-file "
#define WARN_EOF_2	"(wanted `"
#define WARN_EOF_3	"')\n"

#define ERR_ALLOC	"heredoc: Error allocating memory"

static void	warn_eof(const char *delim, size_t delim_len)
{
	static const size_t	lens[] = {54, 9, 3};

	write(STDERR_FILENO, "\n", 1);
	write(STDERR_FILENO, WARN_EOF, lens[0] + 1);
	write(STDERR_FILENO, WARN_EOF_2, lens[1]);
	write(STDERR_FILENO, delim, delim_len);
	write(STDERR_FILENO, WARN_EOF_3, lens[2]);
}

static bool	error(const char *msg, t_dynarr *doc, void *free_)
{
	size_t	n;

	if (doc && doc->arr)
	{
		n = doc->length;
		while (n--)
			free(*((char **) dynarr_get(doc, n)));
		dynarr_delete(doc);
	}
	free(free_);
	if (msg)
		perror(msg);
	return (false);
}

static bool	get_line(char **line)
{
	write(STDOUT_FILENO, ">", 2);
	*line = get_next_line(STDIN_FILENO);
	return (*line != NULL && !g_env.interrupted);
}

static bool	get_lines(t_dynarr *doc, char *delim, size_t delim_len)
{
	char	*line;

	while (get_line(&line))
	{
		if (ft_strncmp(line, delim, delim_len) == 0 && line[delim_len] == '\n')
			break ;
		if (dynarr_addone(doc, &line))
			continue ;
		free(line);
		return (error(ERR_ALLOC, doc, delim));
	}
	if (line == NULL && !g_env.interrupted)
		warn_eof(delim, delim_len);
	free(line);
	return (true);
}

bool	read_heredoc(t_redir *redir, char *delim, size_t delim_len)
{
	const char	*null = NULL;
	t_dynarr	doc;

	if (!dynarr_create(&doc, 32, sizeof(char *)))
		return (error(ERR_ALLOC, NULL, delim));
	if (!get_lines(&doc, delim, delim_len))
		return (false);
	if (g_env.interrupted)
		return (error(NULL, &doc, delim));
	if (!dynarr_addone(&doc, &null))
		return (error(ERR_ALLOC, &doc, delim));
	redir->hd.doc = doc.arr;
	return (true);
}
