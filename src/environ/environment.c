/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_environ.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/08 19:28:03 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/08 19:28:03 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>
#include <stdio.h>

extern char	**environ;

static size_t	get_env_len(void)
{
	size_t	len;

	len = 0;
	while (environ[len])
		++len;
	return (len);
}

static bool	copy_values(t_dynarr *vars, size_t len)
{
	char	**new;
	size_t	i;

	new = vars->arr;
	i = 0;
	while (i < len)
	{
		new[i] = ft_strdup(environ[i]);
		if (new[i++] != NULL)
			continue ;
		ft_free_mult((void **) new);
		return (false);
	}
	new[len] = NULL;
	vars->length = len + 1;
	return (true);
}

uint8_t	init_environment(void)
{
	t_dynarr	*vars;
	size_t		env_len;

	env_len = get_env_len();
	vars = &g_globals.vars;
	if (dynarr_create(vars, env_len + 1, sizeof(char *)))
		if (copy_values(vars, env_len))
			return (EXIT_SUCCESS);
	perror("init_environment");
	return (EXIT_FAILURE);
}

void	clean_environment(void)
{
	char	**env;

	env = g_globals.vars.arr;
	while (*env)
		free(*env++);
	dynarr_delete(&g_globals.vars);
}
