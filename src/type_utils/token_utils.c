/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/31 13:41:43 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/31 13:41:43 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <ms_types.h>

bool	flag(t_flag flags, t_flag flag)
{
	return ((flags & flag) != 0);
}

bool	is_redir(t_tokentype type)
{
	return (type >= RED_IN && type <= RED_APP);
}
