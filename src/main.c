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
#include <stdio.h>
#include <dirent.h>

int	main(int argc, char **argv, char **envp)
{
	while (*argv != NULL)
		printf("%s\n", *argv++);
	minishell(argc, argv, envp);
}
