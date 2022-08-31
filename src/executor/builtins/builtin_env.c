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

uint8_t	builtin_env(char **argv)
{
	const char	**env;

	(void) argv;
	env = g_globals.vars.arr;
	while (*env)
		printf("%s\n", *env++);
	return (SUCCESS);
}
