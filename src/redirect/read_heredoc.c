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
#include <signal.h>

#define WARN_EOF	"bash: warning: here-document delimited by end-of-file "
#define WARN_EOF_2	"(wanted `"
#define WARN_EOF_3	"')\n"

#define ERR_SIGNAL	"heredoc: Error installing signal handler"
#define ERR_OPEN	"heredoc: Error opening heredoc"
#define ERR_WRITE	"heredoc: Error writing to heredoc"

noreturn
static void	warn_eof(int32_t fd, const char *delim, size_t delim_len)
{
	static const size_t	lens[] = {54, 9, 3};

	close(fd);
	write(STDERR_FILENO, "\n", 1);
	write(STDERR_FILENO, WARN_EOF, lens[0]);
	write(STDERR_FILENO, WARN_EOF_2, lens[1]);
	write(STDERR_FILENO, delim, delim_len);
	write(STDERR_FILENO, WARN_EOF_3, lens[2]);
	exit(EXIT_SUCCESS);
}

noreturn
static void	exit_error(int32_t fd, const char *msg, char *file)
{
	if (file)
		unlink(file);
	if (fd >= 0)
		close(fd);
	perror(msg);
	exit(EXIT_FAILURE);
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

noreturn void	read_heredoc(char *file, char *delim, bool expand)
{
	int32_t	fd;
	char	*line;
	size_t	delim_len;

	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		exit_error(-1, ERR_SIGNAL, NULL);
	fd = open(file, O_TRUNC | O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
		exit_error(fd, ERR_OPEN, file);
	delim_len = ft_strlen(delim);
	while (true)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			warn_eof(fd, delim, delim_len);
		if (ft_strncmp(line, delim, delim_len) == 0 && line[delim_len] == '\n')
			break ;
		if (!write_heredoc(fd, line, expand))
			exit_error(fd, ERR_WRITE, file);
	}
	free(line);
	close(fd);
	exit(EXIT_SUCCESS);
}
