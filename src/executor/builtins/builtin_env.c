/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_env.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/10 18:31:22 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/10 18:31:22 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <execute.h>
#include <minishell.h>
#include <stdio.h>

uint8_t	builtin_env(t_cmd_node *cmd)
{
	const char	**env;

	(void) cmd;
	env = g_globals.vars.arr;
	while (*env)
		printf("%s\n", *env++);
	return (SUCCESS);
}
