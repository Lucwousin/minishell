/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/07 21:39:14 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/07 21:39:14 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_sh_env	g_env;

int	main(int argc, char **argv)
{
	return (minishell(argc, argv));
}
