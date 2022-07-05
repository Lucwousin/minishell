/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   matcher.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 13:57:32 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/22 13:57:32 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>
#include <libft.h>

static void	match_operator(const char *cmd, size_t *idx, t_token *token)
{
	char	c;

	token->token = OPERATOR;
	token->start = *idx;
	c = cmd[*idx];
	while (cmd[*idx] == c)
		++(*idx);
}

static void	handle_var(const char *cmd, size_t *idx, t_token *token)
{
	t_token	subtoken;
	char	c;

	subtoken.token = VARIABLE;
	subtoken.start = *idx;
	while (true)
	{
		c = cmd[++(*idx)];
		if (strcontains(BLANK_CHARS, c) || \
			strcontains(OPERATOR_CHARS, c) || \
			strcontains(QUOTE_CHARS, c) || \
			c == VAR_CHAR || c == '\0')
			break ;
	}
	subtoken.end = --(*idx);
	if (!dynarr_addone(&token->sub, &subtoken))
		exit(EXIT_FAILURE); // TODO: error handlign
}

static void	handle_quote(const char *cmd, size_t *idx, t_token *token)
{
	t_token	subtoken;
	char	c;
	char	*close;
	char	*var;

	c = cmd[*idx];
	close = ft_strchr(cmd + *idx + 1, c);
	if (close == NULL)
		return ;
	subtoken.token = QUOTE;
	subtoken.start = *idx;
	if (!dynarr_create(&subtoken.sub, SUB_INIT_SIZE, sizeof(t_token)))
		exit(EXIT_FAILURE); //todo: err handling
	if (c == DOUBLE_QUOTE)
	{
		var = ft_strchr(cmd + *idx + 1, VAR_CHAR);
		if (var != NULL && var < close)
		{
			*idx = var - cmd;
			handle_var(cmd, idx, &subtoken);
		}
	}
	*idx = close - cmd;
	subtoken.end = *idx;
	if (!dynarr_finalize(&subtoken.sub) || \
		!dynarr_addone(&token->sub, &subtoken))
		exit(EXIT_FAILURE); // todo: err handign
}

static void	match_word(const char *cmd, size_t *idx, t_token *token)
{
	if (token->token == END_OF_INPUT)
	{
		if (!dynarr_create(&token->sub, SUB_INIT_SIZE, sizeof(t_token)))
			exit(EXIT_FAILURE); // TODO: better error handling
		token->token = WORD;
		token->start = *idx;
	}
	if (strcontains(QUOTE_CHARS, cmd[*idx]))
		handle_quote(cmd, idx, token);
	else if (cmd[*idx] == VAR_CHAR)
		handle_var(cmd, idx, token);
	*idx = *idx + 1;
	match_token(cmd, idx, token);
}

void	match_token(const char *cmd, size_t *idx, t_token *token)
{
	if (strcontains(BLANK_CHARS, cmd[*idx]) || \
		cmd[*idx] == '\0' || cmd[*idx] == '\n')
	{
		token->end = *idx - 1;
		while (strcontains(BLANK_CHARS, cmd[*idx]))
			++(*idx);
		if (token->token != END_OF_INPUT || \
			cmd[*idx] == '\0' || cmd[*idx] == '\n')
			return ;
	}
	if (strcontains(OPERATOR_CHARS, cmd[*idx]))
	{
		if (token->token == END_OF_INPUT)
			match_operator(cmd, idx, token);
		token->end = *idx - 1;
		return ;
	}
	return (match_word(cmd, idx, token));
}
