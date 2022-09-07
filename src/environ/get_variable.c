/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_variable.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/04 18:13:47 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/04 18:13:47 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>
#include <environ.h>

static const char	*get_exit_str(void)
{
	static char	val[4];
	int32_t		exit_status;
	uint8_t		i;

	exit_status = g_env.exit;
	i = 4;
	val[--i] = '\0';
	while (true)
	{
		val[--i] = "0123456789"[exit_status % 10];
		exit_status /= 10;
		if (exit_status == 0)
			break ;
	}
	return (val + i);
}

const char	**find_variable(const char *var, size_t len)
{
	const char		**env;

	env = g_env.vars.arr;
	while (*env)
	{
		if (ft_strncmp(*env, var, len) == 0 && (*env)[len] == '=')
			return (env);
		++env;
	}
	return (NULL);
}

const char	*get_variable_value(const char *var, size_t len)
{
	const char	**varp;

	if (*var == '?' && len == 1)
		return (get_exit_str());
	varp = find_variable(var, len);
	if (varp == NULL)
		return (NULL);
	return (*varp + len + 1);
}

const char	*get_variable(const char *name)
{
	if (*name == '?')
		return (get_exit_str());
	return (get_variable_value(name, ft_strlen(name)));
}
