/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_unset.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/10 18:31:22 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/10 18:31:22 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <execute.h>
#include <environ.h>
#include <libft.h>

static uint8_t	try_unset(char *str)
{
	t_dynarr	*env;
	const char	**environ;
	const char	**varp;

	varp = find_variable(str, ft_strlen(str));
	if (varp == NULL)
		return (SUCCESS);
	env = &g_globals.vars;
	environ = env->arr;
	ft_memmove(varp, varp + 1, env->length - (varp - environ + 1));
	env->length--;
	return (SUCCESS);
}

uint8_t	builtin_unset(t_cmd_node *cmd)
{
	uint8_t	status;
	char	**args;

	status = SUCCESS;
	args = cmd->argv.arr;
	while (*(++args))
		status |= try_unset(*args);
	return (status);
}
