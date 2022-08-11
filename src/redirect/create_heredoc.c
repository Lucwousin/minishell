/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   create_heredoc.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/04 14:57:34 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/04 14:57:34 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <redir.h>
#include <libft.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define HEX_CHARS		"0123456789abcdef"
#define PREFIX			"/tmp/ms_"
#define PREFIX_L		8
#define NAME_TOO_LONG	"Heredoc delimiter is too long! Can't generate filename"

bool	general_error(const char *where);

static void	add_pointer(char *name, uint64_t pointer)
{
	size_t	n;

	n = sizeof(size_t) * 2;
	while (n--)
	{
		name[n] = HEX_CHARS[pointer % 0x10];
		pointer /= 0x10;
	}
}

static bool	create_name(char *delim, char **dst)
{
	size_t	name_len;
	size_t	delim_len;
	char	*name;

	delim_len = ft_strlen(delim);
	name_len = 9;
	name_len += sizeof(size_t) * 2;
	name_len += delim_len;
	if (name_len >= 0xFF + 6)
		return (ft_putendl_fd(NAME_TOO_LONG, STDERR_FILENO), false);
	name = ft_calloc(name_len, sizeof(char));
	if (name == NULL)
		return (general_error("heredoc create_name"));
	ft_memcpy(name, PREFIX, PREFIX_L);
	ft_memcpy(name + PREFIX_L, delim, delim_len);
	add_pointer(name + PREFIX_L + delim_len, (uint64_t) delim);
	*dst = name;
	return (true);
}

bool	create_heredoc(char **dst, bool expand)
{
	char	*delim;
	pid_t	pid;
	int32_t	status;

	delim = *dst;
	if (!create_name(delim, dst))
		return (false);
	pid = fork();
	if (pid < 0)
		return (free(delim), false);
	if (pid == 0)
		read_heredoc(*dst, delim, expand);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_globals.exit = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		g_globals.exit = WTERMSIG(status) + 0x80;
	if (g_globals.exit == SIGINT + 0x80)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (g_globals.exit == EXIT_SUCCESS);
}
