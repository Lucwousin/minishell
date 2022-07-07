/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_str.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/06 21:44:26 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/06 21:44:26 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <env.h>
#include <libft.h>

bool	parse_env_var(t_dynarr *envs, const char *str)
{
	t_env_var	var;
	char		*eq_p;

	eq_p = ft_strchr(str, '=');
	if (eq_p == NULL)
		return (false); // Maybe exit with status for err msgs?
	var.key = ft_substr(str, 0, eq_p - str);
	var.val = ft_strdup(eq_p + 1);
	if (var.key && var.val)
		return (dynarr_addone(envs, &var));
	free((void *) var.key);
	free((void *) var.val);
	return (false);
}

char	*env_var_to_str(t_env_var *var)
{
	size_t	key_len;
	size_t	val_len;
	char	*str;

	key_len = ft_strlen(var->key);
	val_len = ft_strlen(var->val);
	str = malloc((key_len + val_len + 2) * sizeof(char));
	if (!str)
		return (NULL);
	ft_memcpy(str, var->key, key_len);
	str[key_len] = '=';
	ft_memcpy(str + key_len + 1, var->val, val_len);
	str[key_len + val_len + 1] = '\0';
	return (str);
}
