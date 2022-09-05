/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_cd.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/08 17:44:01 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/08 17:44:01 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>
#include <libft.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define ERR_PREFIX		"cd: "
#define ARGS_NOT_IMPL	"no args cd is not implemented"
#define TOO_MANY_ARGS	"too many arguments"

/*
 * cd on cluster macs does not check argument count. I think that's a useful
 * check, so I'm keeping it in here.
 */
uint8_t	builtin_cd(char **argv)
{
	char	*path;
	char	*err;

	if (argv[1] == NULL)
		return (builtin_err(ERR_PREFIX, NULL, ARGS_NOT_IMPL, ERROR));
	if (argv[2] != NULL)
		return (builtin_err(ERR_PREFIX, NULL, TOO_MANY_ARGS, ERROR));
	path = argv[1];
	if (chdir(path) == SUCCESS)
		return (SUCCESS);
	err = ft_strjoin(path, ": ");
	if (err != NULL)
		path = err;
	builtin_err(ERR_PREFIX, path, strerror(errno), ERROR);
	free(err);
	return (ERROR);
}
