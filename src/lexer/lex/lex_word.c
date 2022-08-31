/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lex_word.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/31 13:45:59 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/31 13:45:59 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>

static bool	set_flag_and_consume(t_lexer *lexer, t_flag flag)
{
	lexer->flags |= flag;
	return (consume_char(lexer));
}

static bool	can_append_next(t_lexer *lexer)
{
	t_char_type	next_type;

	next_type = get_type(lexer->str + lexer->idx);
	return (next_type != OPERATOR_S \
			&& next_type != WHITE_S \
			&& next_type != EOF_S);
}

static bool	set_quotes(t_lexer *lexer, t_char_type type)
{
	const bool	double_q = type == DQUOTE_S;
	const bool	is_quoted = flag(lexer->flags, IS_QUOTED);

	if ((double_q && lexer->flags & S_QUOTED) \
	|| (!double_q && lexer->flags & D_QUOTED))
		return (consume_char(lexer));
	if (lexer->idx == lexer->token_start)
	{
		if (double_q)
			return (set_flag_and_consume(lexer, D_QUOTED));
		else
			return (set_flag_and_consume(lexer, S_QUOTED));
	}
	if (is_quoted)
		consume_char(lexer);
	if (!is_quoted || can_append_next(lexer))
		lexer->flags |= APPEND_NEXT;
	return (switch_state(lexer, DEFAULT));
}

static bool	set_glob(t_lexer *lexer)
{
	if (lexer->idx != lexer->token_start)
	{
		lexer->flags |= APPEND_NEXT;
		return (switch_state(lexer, DEFAULT));
	}
	lexer->flags |= HAS_GLOB;
	return (consume_char(lexer));
}

bool	lex_word(t_lexer *lexer, t_char_type type)
{
	if (type == DQUOTE_S || type == SQUOTE_S)
		return (set_quotes(lexer, type));
	if (type == WORD_S)
		return (consume_char(lexer));
	if (type == VAR_S && !(lexer->flags & S_QUOTED))
		return (set_flag_and_consume(lexer, HAS_VAR));
	if (lexer->flags & IS_QUOTED && type != EOF_S)
		return (consume_char(lexer));
	if (type == WILDCARD_S)
		return (set_glob(lexer));
	return (switch_state(lexer, DEFAULT));
}
