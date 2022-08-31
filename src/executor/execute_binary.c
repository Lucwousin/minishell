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

#define ERR_MES	"Error during execution"

static noreturn void	exit_(uint8_t status)
{
	if (errno != 0)
		perror(ERR_MES);
	exit(status);
}

static char	*find_executable(char *name)
{
	char	**path;
	char	**path_c;
	char	*cur;

	if (ft_strchr(name, '/') != NULL)
		return (ft_strdup(name));
	name = ft_strjoin("/", name);
	if (!name)
		return (NULL);
	path = ft_split(get_variable("PATH"), ':');
	if (path == NULL)
		return (free(name), NULL);
	path_c = path;
	while (*path)
	{
		cur = ft_strjoin(*path, name);
		if (cur == NULL || access(cur, X_OK) == 0)
			break ;
		free(cur);
		cur = NULL;
		path++;
	}
	free(name);
	ft_free_mult((void **) path_c);
	return (cur);
}

noreturn void	execute_binary(char **argv)
{
	char	*path;

	errno = 0;
	if (*argv == NULL)
		exit_(SUCCESS);
	path = find_executable(argv[0]);
	if (path == NULL)
		exit_(127);
	execve(path, argv, g_globals.vars.arr);
	free(path);
	exit_(126);
}
