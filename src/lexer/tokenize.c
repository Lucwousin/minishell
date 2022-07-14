/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 13:57:10 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/22 13:57:10 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <token.h>
#include <dynarr.h>

static t_lexerfunc	g_lex[] = {
[DEFAULT] = switch_state,
[WHITE_S] = lex_simple,
[WORD_S] = lex_simple,
[SQUOTE_S] = lex_simple_single,
[DQUOTE_S] = lex_simple_single,
[PAR_OPEN_S] = lex_simple_single,
[PAR_CLOSE_S] = lex_simple_single,
[OPERATOR_S] = lex_operator,
[VAR_S] = lex_variable,
};

static t_lex_state	get_type(const char *c)
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

void	tokenize(t_dynarr *tokens, const char *cmd)
{
	t_lexer		lexer;

	lexer.state = DEFAULT;
	lexer.current_token.token = END_OF_INPUT;
	lexer.current_token.start = 0;
	lexer.tokens = tokens;
	lexer.str = cmd;
	lexer.idx = 0;
	if (!dynarr_create(lexer.tokens, TOKENS_INIT_SIZE, sizeof(t_token)))
		exit(EXIT_FAILURE); // TODO: error
	while (lexer.state != EOF_S)
		if (!g_lex[lexer.state](&lexer, get_type(lexer.str + lexer.idx)))
			return ; //todo: An error occured somewhere!
	if (!dynarr_finalize(lexer.tokens))
		return ; // TODO: error
}

static bool	delimit_token(t_lexer *lexer)
{
	if (lexer->idx == lexer->current_token.start)
		lexer->current_token.end = lexer->idx;
	else
		lexer->current_token.end = lexer->idx - 1;
	return (dynarr_addone(lexer->tokens, &lexer->current_token));
}

bool	switch_state(t_lexer *lexer, t_lex_state new_state)
{
	static const t_tokentype	types[] = {
	[DEFAULT] = END_OF_INPUT,
	[WHITE_S] = WHITESPACE,
	[OPERATOR_S] = OPERATOR,
	[WORD_S] = WORD,
	[SQUOTE_S] = SQUOTE,
	[DQUOTE_S] = DQUOTE,
	[PAR_OPEN_S] = PAR_OPEN,
	[PAR_CLOSE_S] = PAR_CLOSE,
	[VAR_S] = VARIABLE
	};
	bool						very_long_return_value_thanks_norminette;

	if (new_state == DEFAULT || new_state == EOF_S)
		very_long_return_value_thanks_norminette = delimit_token(lexer);
	else
		very_long_return_value_thanks_norminette = true;
	if (new_state != EOF_S)
	{
		lexer->current_token.token = types[new_state];
		lexer->current_token.start = lexer->idx;
	}
	lexer->state = new_state;
	return (very_long_return_value_thanks_norminette);
}

bool	consume_char(t_lexer *lexer)
{
	lexer->idx++;
	return (true);
}
