/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lex_operator.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/11 23:40:50 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/11 23:40:50 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>

static bool	can_append(t_lexer *lexer)
{
	char	prev;

	prev = lexer->str[lexer->token_start];
	if (prev == '(' || prev == ')')
		return (false);
	if (prev != lexer->str[lexer->idx])
		return (false);
	return (true);
}

bool	lex_operator(t_lexer *lexer, t_char_type type)
{
	if (type == OPERATOR_S && lexer->idx <= lexer->token_start + 1)
		if (lexer->idx == lexer->token_start || can_append(lexer))
			return (consume_char(lexer));
	return (switch_state(lexer, DEFAULT));
}
