/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   variable_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/09 19:32:37 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/09 19:32:37 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*var_name_end(const char *str)
{
	if (!ft_isdigit(*str))
		while (ft_isalnum(*str) || *str == '_')
			++str;
	return ((char *)str);
}
