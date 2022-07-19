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
#include <libft.h>

static bool	is_first_name_idx(t_lexer *lexer)
{
	return (lexer->idx == lexer->current_token.start + 1);
}

static bool	is_valid_start_char(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static bool	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

bool	lex_variable(t_lexer *lexer, t_char_type type)
{
	if (type != VAR_S && type != WORD_S)
		return (switch_state(lexer, DEFAULT));
	if (type == VAR_S)
	{
		if (lexer->idx == lexer->current_token.start)
			return (consume_char(lexer));
		if (is_first_name_idx(lexer))
			consume_char(lexer);
		return (switch_state(lexer, DEFAULT));
	}
	if (is_first_name_idx(lexer))
	{
		if (is_valid_start_char(lexer->str[lexer->idx]))
			return (consume_char(lexer));
		if (lexer->str[lexer->idx] == '?' || lexer->str[lexer->idx] == '$')
			return (consume_char(lexer), switch_state(lexer, DEFAULT));
		return (consume_char(lexer), set_state(lexer, WORD_S));
	}
	if (is_valid_var_char(lexer->str[lexer->idx]))
		return (consume_char(lexer));
	return (switch_state(lexer, DEFAULT));
}
