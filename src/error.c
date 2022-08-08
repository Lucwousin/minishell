/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/15 17:58:07 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/15 17:58:07 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <input.h>
#include <libft.h>
#include <unistd.h>
#include <stdio.h>

#define ERROR_MSG_PREFIX	"Minishell: Error in "
#define SYNTAX_MSG_PREFIX	"Minishell: Syntax error near unexpected token `"
#define SYNTAX_MSG_SUFFIX	"'\n"
#define NEWLINE_NAME		"newline"

bool	general_error(const char *where)
{
	ft_putstr_fd(ERROR_MSG_PREFIX, STDERR_FILENO);
	ft_putstr_fd((char *) where, STDERR_FILENO);
	perror(where);
	return (false);
}

void	syntax_error(const char *where)
{
	ft_putstr_fd(SYNTAX_MSG_PREFIX, STDERR_FILENO);
	if (*where == '\n')
		ft_putstr_fd(NEWLINE_NAME, STDERR_FILENO);
	else
		ft_putstr_fd((char *) where, STDERR_FILENO);
	ft_putstr_fd(SYNTAX_MSG_SUFFIX, STDERR_FILENO);
}

void	syntax_error_type(t_tokentype type)
{
	static const char	*strs[] = {
	[END_OF_INPUT] = "\n",
	[PAR_OPEN] = "(",
	[PAR_CLOSE] = ")",
	[OR] = "||",
	[AND] = "&&",
	[PIPE] = "|",
	};

	return (syntax_error(strs[type]));
}
