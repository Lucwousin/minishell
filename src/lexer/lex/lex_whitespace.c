/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lex_whitespace.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/11 23:40:51 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/11 23:40:51 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>

bool	lex_whitespace(t_lexer *lexer, t_char_type type)
{
	if (type == WHITE_S)
		return (consume_char(lexer));
	if (type == EOF_S)
		lexer->state = END_OF_INPUT;
	return (switch_state(lexer, type));
}
