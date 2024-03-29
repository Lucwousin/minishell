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
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
		}
	}
	exit(status);
}

static char	*search_path(char *name, char **path)
{
	const char	*orig_arg = name;
	char		*result;

	name = ft_strjoin("/", orig_arg);
	if (name == NULL)
		return (NULL);
	while (*path)
	{
		result = ft_strjoin(*path, name);
		if (result == NULL || access(result, X_OK) == 0)
			break ;
		free(result);
		result = NULL;
		errno = 0;
		++path;
	}
	free(name);
	return (result);
}

static char	*find_executable(char *orig_arg)
{
	char	**path;
	char	*result;

	if (ft_strchr(orig_arg, '/') != NULL)
	{
		if (access(orig_arg, X_OK) == 0)
			return (ft_strdup(orig_arg));
		return (NULL);
	}
	path = ft_split(get_variable("PATH"), ':');
	if (path != NULL && *path != NULL)
		result = search_path(orig_arg, path);
	else if (access(orig_arg, X_OK) == 0)
		result = ft_strdup(orig_arg);
	else
		result = NULL;
	if (path != NULL)
		ft_free_mult((void **) path);
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
	execve(path, argv, g_env.vars.arr);
	free(path);
	exit_(argv[0], 126);
}
