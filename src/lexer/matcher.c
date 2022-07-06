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

void	match_operator(const char *cmd, size_t *idx, t_token *token)
{
	char	c;
	size_t	len;

	token->token = OPERATOR;
	token->start = *idx;
	c = cmd[*idx];
	while (cmd[*idx] == c)
		++(*idx);
	len = *idx - token->start;
	if (ft_strncmp(PIPE_STR, cmd + token->start, len) == 0)
		token->token = PIPE;
	else if (ft_strncmp(RED_IN_STR, cmd + token->start, len) == 0)
		token->token = RED_IN;
	else if (ft_strncmp(RED_HD_STR, cmd + token->start, len) == 0)
		token->token = RED_HD;
	else if (ft_strncmp(RED_OUT_STR, cmd + token->start, len) == 0)
		token->token = RED_OUT;
	else if (ft_strncmp(RED_APPEND_STR, cmd + token->start, len) == 0)
		token->token = RED_APP;
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
			c == VAR_CHAR || c == '\0' || c == '\n')
			break ;
	}
	subtoken.end = --(*idx);
	if (!dynarr_addone(&token->sub, &subtoken))
		exit(EXIT_FAILURE); // TODO: error handlign
}

static void	find_vars(
		const char *cmd, size_t *idx, t_token *token, const char *close)
{
	char	*var;

	while (true)
	{
		var = ft_strchr(cmd + *idx + 1, VAR_CHAR);
		if (var == NULL || var >= close)
			break ;
		*idx = var - cmd;
		handle_var(cmd, idx, token);
	}
}

static void	handle_quote(const char *cmd, size_t *idx, t_token *token)
{
	t_token	subtoken;
	char	*close;

	close = ft_strchr(cmd + *idx + 1, cmd[*idx]);
	if (close == NULL)
		return ;
	subtoken.token = QUOTE;
	subtoken.start = *idx;
	if (!dynarr_create(&subtoken.sub, SUB_INIT_SIZE, sizeof(t_token)))
		exit(EXIT_FAILURE); //todo: err handling
	if (cmd[*idx] == DOUBLE_QUOTE)
		find_vars(cmd, idx, &subtoken, close);
	*idx = close - cmd;
	subtoken.end = *idx;
	if (!dynarr_finalize(&subtoken.sub) || \
		!dynarr_addone(&token->sub, &subtoken))
		exit(EXIT_FAILURE); // todo: err handign
}

void	match_word(const char *cmd, size_t *idx, t_token *token)
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
	(*idx)++;
	match_token(cmd, idx, token);
}
