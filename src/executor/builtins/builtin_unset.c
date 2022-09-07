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
	const char	**environ;
	const char	**varp;
	t_dynarr	*env;
	size_t		to_move;

	varp = find_variable(str, ft_strlen(str));
	if (varp == NULL)
		return (SUCCESS);
	free((char *) *varp);
	env = &g_env.vars;
	environ = env->arr;
	to_move = env->length - (varp - environ + 1);
	ft_memmove(varp, varp + 1, to_move * sizeof(char *));
	env->length--;
	return (SUCCESS);
}

uint8_t	builtin_unset(char **argv)
{
	uint8_t	status;

	status = SUCCESS;
	while (*(++argv))
		status |= try_unset(*argv);
	return (status);
}
