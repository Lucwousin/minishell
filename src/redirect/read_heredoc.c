/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   read_heredoc.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/04 16:15:54 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/04 16:15:54 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>
#include <redir.h>
#include <get_next_line.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define GNL_NULL_ERR	"Error reading stdin for heredoc:"
#define WRITE_ERR		"Error writing to heredoc:"

static bool	exit_error(int32_t fd, const char *msg)
{
	close(fd);
	perror(msg);
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

static bool	write_heredoc(int32_t fd, char *line, bool expand)
{
	size_t	write_len;
	char	*l;

	l = line;
	write_len = 0;
	while (l[write_len])
	{
		if (!expand || l[write_len] != '$')
		{
			++write_len;
			continue ;
		}
		if (write(fd, l, write_len) < 0)
			return (free(line), false);
		l += write_len;
		if (!write_variable(fd, &l))
			return (free(line), false);
		write_len = 0;
	}
	if (write(fd, l, write_len) < 0)
		return (free(line), false);
	free(line);
	return (true);
}

bool	read_heredoc(char *file, char *delim, bool expand)
{
	int32_t	fd;
	char	*line;
	size_t	delim_len;

	fd = open(file, O_TRUNC | O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
		return (false);
	delim_len = ft_strlen(delim);
	while (true)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			return (exit_error(fd, GNL_NULL_ERR));
		if (ft_strncmp(line, delim, delim_len) == 0 && line[delim_len] == '\n')
			break ;
		if (!write_heredoc(fd, line, expand))
			return (exit_error(fd, WRITE_ERR));
	}
	free(line);
	close(fd);
	return (true);
}
