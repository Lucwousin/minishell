/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_binary.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/05 19:32:37 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/05 19:32:37 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <execute.h>
#include <environ.h>
#include <libft.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdnoreturn.h>

static noreturn void	exit_(char *arg, uint8_t status)
{
	if (status != 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		if (errno)
			perror(arg);
		else if (status == 127)
		{
			ft_putstr_fd(arg, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		}
	}
	exit(status);
}

static char	*search_path(char *name)
{
	char	**path;
	char	**p;
	char	*result;

	path = ft_split(get_variable("PATH"), ':');
	if (path == NULL)
		return (NULL);
	p = (char **) path;
	while (*p)
	{
		result = ft_strjoin(*p, name);
		if (result == NULL || access(result, X_OK) == 0)
			break ;
		free(result);
		result = NULL;
		++p;
	}
	ft_free_mult((void **) path);
	return (result);
}

static char	*find_executable(char *name)
{
	char	*result;

	if (ft_strchr(name, '/') != NULL)
		return (ft_strdup(name));
	name = ft_strjoin("/", name);
	if (!name)
		return (NULL);
	result = search_path(name);
	free(name);
	return (result);
}

noreturn void	execute_binary(char **argv)
{
	char	*path;

	errno = 0;
	if (*argv == NULL)
		exit_(NULL, SUCCESS);
	path = find_executable(argv[0]);
	if (path == NULL)
		exit_(argv[0], 127);
	execve(path, argv, g_globals.vars.arr);
	free(path);
	exit_(argv[0], 126);
}
