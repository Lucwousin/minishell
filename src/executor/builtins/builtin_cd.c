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
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define ERR_PREFIX		"cd: "
#define ARGS_NOT_IMPL	"no args cd is not implemented"

uint8_t	builtin_cd(char **argv)
{
	char	*path;

	if (argv[1] == NULL)
		return (builtin_err(ERR_PREFIX, NULL, ARGS_NOT_IMPL, ERROR));
	path = argv[1];
	if (chdir(path) != SUCCESS)
		return (builtin_err(ERR_PREFIX, path, strerror(errno), ERROR));
	return (SUCCESS);
}
