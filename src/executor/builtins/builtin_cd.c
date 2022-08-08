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

#include <execute.h>
#include <unistd.h>
#include <stdio.h>
#include "libft.h"

#define ERR_PREFIX		"cd: "
#define ARGS_NOT_IMPL	"no args cd is not implemented"

static uint8_t	error(char *what, char *arg, bool fail)
{
	ft_putstr_fd(ERR_PREFIX, STDERR_FILENO);
	if (what)
		ft_putstr_fd(what, STDERR_FILENO);
	if (fail)
		perror(arg);
	else
		ft_putchar_fd('\n', STDERR_FILENO);
	return (ERROR);
}

uint8_t	builtin_cd(t_cmd_node *cmd)
{
	char	*path;

	if (cmd->argv.length < 3)
		return (error(ARGS_NOT_IMPL, NULL, false));
	path = ((char **) cmd->argv.arr)[1];
	if (chdir(path) != SUCCESS)
		return (error(NULL, path, true));
	return (SUCCESS);
}
