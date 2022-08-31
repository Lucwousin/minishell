/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   write_heredoc.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/25 15:47:50 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/25 15:47:50 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <redir.h>
#include <environ.h>
#include <libft.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

static bool	error(int32_t fd, char *file)
{
	unlink(file);
	perror(file);
	close(fd);
	return (false);
}

static bool	write_variable(int32_t fd, char **linep)
{
	char		*line;
	char		*var_end;
	const char	*var_value;

	line = (*linep) + 1;
	if (*line == '?' || *line == '$')
		var_end = line + 1;
	else
		var_end = var_name_end(line);
	*linep = var_end;
	if (var_end == line)
		return (write(fd, "$", 1));
	var_value = get_variable_value(line, var_end - line);
	if (var_value == NULL)
		return (true);
	return (write(fd, var_value, ft_strlen(var_value)) >= 0);
}

static bool	write_line(int32_t fd, char *line, bool expand)
{
	size_t	write_len;

	write_len = 0;
	if (!expand)
		write_len = ft_strlen(line);
	while (line[write_len])
	{
		if (line[write_len] != '$')
		{
			++write_len;
			continue ;
		}
		if (write(fd, line, write_len) < 0)
			return (false);
		line += write_len;
		if (!write_variable(fd, &line))
			return (false);
		write_len = 0;
	}
	if (write(fd, line, write_len) < 0)
		return (false);
	return (true);
}

bool	write_heredoc(t_redir *hd)
{
	const bool	expand = hd->type == RED_HD;
	char		**p;
	int32_t		file;

	file = open(hd->hd.file, O_CREAT | O_TRUNC | O_WRONLY, 00644);
	if (file < 0)
		return (error(file, hd->hd.file));
	p = hd->hd.doc;
	while (*p)
		if (!write_line(file, *p++, expand))
			return (error(file, hd->hd.file));
	close(file);
	return (true);
}
