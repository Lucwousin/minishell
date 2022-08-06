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
#include <stdint.h>
#include <stdlib.h>

static const char	*get_exit_str(void)
{
	static char	val[4];
	int32_t		exit_status;
	uint8_t		i;

	exit_status = g_globals.exit;
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

const char	*get_variable(const char *name)
{
	if (*name == '?')
		return (get_exit_str());
	return (getenv(name));
}
