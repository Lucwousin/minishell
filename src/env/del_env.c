/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   del_env.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/07 18:08:36 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/07 18:08:36 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stdlib.h>

static void	delete_env_var(void *var_ptr, void *ignored)
{
	t_env_var	*var;

	(void) ignored;
	var = var_ptr;
	free(var->key);
	free(var->val);
}

void	delete_environment(t_dynarr *envp)
{
	dynarr_foreach(envp, delete_env_var, NULL);
	dynarr_delete(envp);
}
