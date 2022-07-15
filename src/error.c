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
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>

#define MALLOC_MSG_PREFIX	"Minishell: Allocation failed in "
#define SYNTAX_MSG_PREFIX	"Minishell: Syntax error near unexpected token `"
#define SYNTAX_MSG_SUFFIX	"'\n"
#define NEWLINE_NAME		"newline"

bool	malloc_error(const char *where)
{
	ft_putstr_fd(MALLOC_MSG_PREFIX, STDERR_FILENO);
	ft_putstr_fd((char *) where, STDERR_FILENO);
	perror(where);
	return (false);
}

bool	syntax_error(const char *cmd, t_token *token)
{
	char	*substr;

	substr = ft_substr(cmd, token->start, token->end - token->start + 1);
	if (substr == NULL)
		return (malloc_error("syntax_error"));
	ft_putstr_fd(SYNTAX_MSG_PREFIX, STDERR_FILENO);
	if (*substr == '\n')
		ft_putstr_fd(NEWLINE_NAME, STDERR_FILENO);
	else
		ft_putstr_fd(substr, STDERR_FILENO);
	ft_putstr_fd(SYNTAX_MSG_SUFFIX, STDERR_FILENO);
	free(substr);
	return (false);
}
