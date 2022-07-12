/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lex_simple.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/11 23:40:51 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/11 23:40:51 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>

bool	lex_simple(t_lexer *lexer, t_char_type type)
{
	if (type == lexer->state)
		return (consume_char(lexer));
	return (switch_state(lexer, DEFAULT));
}

bool	lex_simple_single(t_lexer *lexer, t_char_type type)
{
	(void) type;
	consume_char(lexer);
	return (switch_state(lexer, DEFAULT));
}
