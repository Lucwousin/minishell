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
#include <libft.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define ERR_MES	"Error during execution"

extern char	**environ;

static void	exit_(uint8_t status)
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
	path = ft_split(getenv("PATH"), ':');
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

void	execute_binary(t_cmd_node *cmd)
{
	char	*path;

	errno = 0;
	if (cmd->argv.length == 0)
		exit_(0);
	path = find_executable(*((char **) dynarr_get(&cmd->argv, 0)));
	if (path == NULL)
		exit_(127);
	execve(path, cmd->argv.arr, environ);
	free(path);
	exit_(126);
}
