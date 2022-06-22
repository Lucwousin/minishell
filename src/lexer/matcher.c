/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   matcher.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: user <user@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 13:57:32 by user          #+#    #+#                 */
/*   Updated: 2022/06/22 13:57:32 by user          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "libft.h"

static void	match_operator(const char *cmd, size_t *idx, t_codepoint *token)
{
	char	c;

	token->token = OPERATOR;
	token->start = *idx;
	c = cmd[*idx];
	while (cmd[*idx] == c)
		++(*idx);
	token->end = *idx - 1;
}

static void	match_word(const char *cmd, size_t *idx, t_codepoint *token)
{
	char	c;
	char	*close_quote;

	if (token->token == END_OF_INPUT)
	{
		token->token = WORD;
		token->start = *idx;
	}
	c = cmd[(*idx)++];
	if (strcontains(QUOTE_CHARS, c))
	{
		close_quote = ft_strchr(cmd + *idx, c);
		if (close_quote != NULL)
			*idx = close_quote - cmd + 1;
	}
	match_token(cmd, idx, token);
	token->end = *idx - 1;
}

void	match_token(const char *cmd, size_t *idx, t_codepoint *token)
{
	if (cmd[*idx] == '\0' || cmd[*idx] == '\n')
		return ;
	if (strcontains(BLANK_CHARS, cmd[*idx]))
	{
		if (token->token != END_OF_INPUT)
			return ;
		while (strcontains(BLANK_CHARS, cmd[*idx]))
			++(*idx);
	}
	if (strcontains(OPERATOR_CHARS, cmd[*idx]))
	{
		if (token->token == END_OF_INPUT)
			match_operator(cmd, idx, token);
		return ;
	}
	return (match_word(cmd, idx, token));
}
