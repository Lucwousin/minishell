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

bool	lex_operator(t_lexer *lexer, t_char_type type)
{
	if (type == OPERATOR_S)
		if (lexer->idx == lexer->current_token.start || \
			lexer->str[lexer->idx - 1] == lexer->str[lexer->idx])
			return (consume_char(lexer));
	return (switch_state(lexer, DEFAULT));
}
