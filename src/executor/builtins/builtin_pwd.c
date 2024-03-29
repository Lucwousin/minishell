/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_pwd.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/08 18:11:37 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/08 18:11:37 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <execute.h>
#include <unistd.h>
#include <libft.h>
#include <stdio.h>

uint8_t	builtin_pwd(char **argv)
{
	char	*cwd;

	(void) argv;
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("pwd");
		return (ERROR);
	}
	else
	{
		printf("%s\n", cwd);
		free(cwd);
		return (SUCCESS);
	}
}
