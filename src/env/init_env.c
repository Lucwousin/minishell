/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_env.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/06 21:38:45 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/06 21:38:45 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	init_environment(t_msh *msh)
{
	extern char	**environ;
	size_t		index;

	if (!dynarr_create(&msh->envp, ENV_INIT_SIZE, sizeof(t_env_var)))
		return (false);
	index = 0;
	while (environ[index] != NULL)
	{
		if (parse_env_var(&msh->envp, environ[index++]))
			continue ;
		dynarr_delete(&msh->envp);
		return (false);
	}
	return (true);
}
