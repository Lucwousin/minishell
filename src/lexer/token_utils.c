/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/15 17:26:56 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/15 17:26:56 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>
#include <libft.h>

#define OPERATOR_RED_IN		"<"
#define OPERATOR_RED_HD		"<<"
#define OPERATOR_RED_OUT	">"
#define OPERATOR_RED_APP	">>"
#define OPERATOR_PIPE		"|"
#define OPERATOR_OR			"||"
#define OPERATOR_AND		"&&"

bool	add_token(const char *cmd, t_dynarr *buf, t_token *token)
{
	size_t	len;

	len = token->end - token->start + 1;
	return (dynarr_add(buf, cmd + token->start, len));
}

bool	toggle_quote(bool in_quote[2], t_tokentype type)
{
	if (type != DQUOTE && type != SQUOTE)
		return (false);
	if (in_quote[type == SQUOTE])
		return (false);
	in_quote[type != SQUOTE] = !in_quote[type != SQUOTE];
	return (true);
}

void	id_operator(const char *cmd, t_token *token, t_token *next)
{
	size_t		len;

	len = token->end - token->start + 1;
	if (ft_strncmp(OPERATOR_RED_IN, cmd + token->start, len) == 0)
		token->type = RED_IN;
	else if (ft_strncmp(OPERATOR_RED_HD, cmd + token->start, len) == 0)
		token->type = RED_HD;
	else if (ft_strncmp(OPERATOR_RED_OUT, cmd + token->start, len) == 0)
		token->type = RED_OUT;
	else if (ft_strncmp(OPERATOR_RED_APP, cmd + token->start, len) == 0)
		token->type = RED_APP;
	else if (ft_strncmp(OPERATOR_PIPE, cmd + token->start, len) == 0)
		token->type = PIPE;
	else if (ft_strncmp(OPERATOR_OR, cmd + token->start, len) == 0)
		token->type = OR;
	else
		token->type = AND;
	if (token->type == RED_HD && (next->type == SQUOTE || next->type == DQUOTE))
		token->type = RED_HD_Q;
}

t_char_type	get_type(const char *c)
{
	if (ft_strchr(EOF_CHARS, *c) != NULL)
		return (EOF_S);
	if (ft_strchr(BLANK_CHARS, *c) != NULL)
		return (WHITE_S);
	if (ft_strchr(OPERATOR_CHARS, *c) != NULL)
		return (OPERATOR_S);
	if (*c == SINGLE_QUOTE)
		return (SQUOTE_S);
	if (*c == DOUBLE_QUOTE)
		return (DQUOTE_S);
	if (*c == VAR_CHAR && get_type(c + 1) == WORD_S)
		return (VAR_S);
	if (*c == PAR_OPEN_CHAR)
		return (PAR_OPEN_S);
	if (*c == PAR_CLOSE_CHAR)
		return (PAR_CLOSE_S);
	return (WORD_S);
}
