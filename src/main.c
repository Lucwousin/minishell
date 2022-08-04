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

t_global	g_globals;

int	main(int argc, char **argv, char **envp)
{
	g_globals = (t_global){0};
	minishell(argc, argv, envp);
}
