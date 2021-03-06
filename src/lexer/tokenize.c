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

#include <token.h>
#include <dynarr.h>

/**
 * Lexer state to handler function
 */
static t_lexerfunc			g_lex[] = {
[DEFAULT] = switch_state,
[WHITE_S] = lex_simple,
[WORD_S] = lex_simple,
[SQUOTE_S] = lex_simple_single,
[DQUOTE_S] = lex_simple_single,
[PAR_OPEN_S] = lex_simple_single,
[PAR_CLOSE_S] = lex_simple_single,
[OPERATOR_S] = lex_operator,
[VAR_S] = lex_variable,
[WILDCARD_S] = lex_simple_single
};

/**
 * Lexer state/char type to token type
 */
static const t_tokentype	g_types[] = {
[DEFAULT] = END_OF_INPUT,
[WHITE_S] = WHITESPACE,
[OPERATOR_S] = OPERATOR,
[WORD_S] = WORD,
[SQUOTE_S] = SQUOTE,
[DQUOTE_S] = DQUOTE,
[PAR_OPEN_S] = PAR_OPEN,
[PAR_CLOSE_S] = PAR_CLOSE,
[VAR_S] = VARIABLE,
[WILDCARD_S] = GLOB
};

bool	tokenize(t_dynarr *tokens, const char *cmd)
{
	t_lexer	lexer;
	bool	rv;

	lexer.state = DEFAULT;
	lexer.current_token.type = END_OF_INPUT;
	lexer.current_token.start = 0;
	lexer.tokens = tokens;
	lexer.str = cmd;
	lexer.idx = 0;
	rv = true;
	if (!dynarr_create(lexer.tokens, TOKENS_INIT_SIZE, sizeof(t_token)))
		return (false);
	while (lexer.state != EOF_S)
	{
		if (g_lex[lexer.state](&lexer, get_type(lexer.str + lexer.idx)))
			continue ;
		rv = false;
		break ;
	}
	if (rv && dynarr_finalize(lexer.tokens))
		return (true);
	dynarr_delete(tokens);
	return (false);
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
	bool	rv;

	if (new_state == DEFAULT || new_state == EOF_S)
		rv = delimit_token(lexer);
	else
		rv = true;
	if (new_state != EOF_S)
	{
		lexer->current_token.type = g_types[new_state];
		lexer->current_token.start = lexer->idx;
	}
	lexer->state = new_state;
	return (rv);
}

bool	set_state(t_lexer *lexer, t_lex_state new_state)
{
	lexer->state = new_state;
	lexer->current_token.type = g_types[new_state];
	return (true);
}

bool	consume_char(t_lexer *lexer)
{
	lexer->idx++;
	return (true);
}
