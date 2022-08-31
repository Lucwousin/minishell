/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   set_variable.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/09 18:13:47 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/09 18:13:47 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>
#include <minishell.h>
#include <libft.h>
#include <stdio.h>

static uint8_t	add_variable(char *var)
{
	char		**arr;
	size_t		index;

	index = g_globals.vars.length - 1;
	if (dynarr_grow(&g_globals.vars, ++g_globals.vars.length))
	{
		arr = g_globals.vars.arr;
		arr[index] = ft_strdup(var);
		arr[index + 1] = NULL;
		if (arr[index] != NULL)
			return (EXIT_SUCCESS);
	}
	perror("add_variable");
	return (EXIT_FAILURE);
}

uint8_t	set_variable(char *var, char *name_end)
{
	char	**dst;
	char	*old_value;

	dst = (char **) find_variable(var, name_end - var);
	if (dst == NULL)
		return (add_variable(var));
	old_value = *dst;
	*dst = ft_strdup(var);
	if (*dst == NULL)
	{
		*dst = old_value;
		perror("set_variable");
		return (EXIT_FAILURE);
	}
	free(old_value);
	return (EXIT_SUCCESS);
}
