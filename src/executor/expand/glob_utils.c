/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   glob_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/31 14:40:29 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/31 14:40:29 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

bool	open_cwd(DIR **dst)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		*dst = opendir(cwd);
		free(cwd);
		if (*dst != NULL)
			return (true);
	}
	perror("open_cwd");
	return (false);
}
