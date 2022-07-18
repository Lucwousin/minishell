/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lex_variable.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/11 23:40:52 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/11 23:40:52 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>

bool	lex_variable(t_lexer *lexer, t_char_type type)
{
	if (type == WORD_S)
	{
		consume_char(lexer);
		if (lexer->str[lexer->idx] != '?' || \
			lexer->idx != lexer->current_token.start + 1)
			return (true);
	}
	else if (type == VAR_S)
	{
		if (lexer->idx == lexer->current_token.start)
			return (consume_char(lexer));
		if (lexer->idx == lexer->current_token.start + 1)
			consume_char(lexer);
	}
	return (switch_state(lexer, DEFAULT));
}
