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

#include <input.h>
#include <token.h>
#include <stdio.h>
#include <unistd.h>
#include "libft.h"

#define Q_WARN1	"warning: everything after an unclosed quote "
#define Q_WARN2	"will be considered quoted\n"

/**
 * Token type to handler function
 */
static t_lexerfunc			g_lex[] = {
[NO_TYPE] = switch_state,
[WHITESPACE] = lex_whitespace,
[OPERATOR] = lex_operator,
[WORD] = lex_word
};

/**
 * Lexer state/char type to token type
 */
static const t_tokentype	g_types[] = {
[DEFAULT] = NO_TYPE,
[WHITE_S] = WHITESPACE,
[OPERATOR_S] = OPERATOR,
[WORD_S] = WORD,
[SQUOTE_S] = WORD,
[DQUOTE_S] = WORD,
[VAR_S] = WORD,
[WILDCARD_S] = WORD,
[EOF_S] = END_OF_INPUT
};

static void	warn_unclosed_quote(t_lexer *lexer)
{
	t_token	*token;

	token = dynarr_get(lexer->tokens, lexer->tokens->length - 2);
	if (token == NULL)
		return ;
	if (!(token->flags & IS_QUOTED))
		return ;
	if (lexer->str[token->end] == '\'' && token->flags & S_QUOTED)
		return ;
	if (lexer->str[token->end] == '"' && token->flags & D_QUOTED)
		return ;
	token->flags |= UNCLOSED_QUOTE;
	ft_putstr_fd(Q_WARN1 Q_WARN2, STDERR_FILENO);
	ft_putstr_fd("-->> ", STDERR_FILENO);
	ft_putstr_fd(lexer->str + token->start, STDERR_FILENO);
	ft_putstr_fd(" <<--\n", STDERR_FILENO);
}

uint8_t	tokenize(t_dynarr *tokens, const char *cmd)
{
	t_lexer	lexer;

	lexer = (t_lexer){.tokens = tokens, .str = cmd};
	if (!dynarr_create(lexer.tokens, TOKENS_INIT_SIZE, sizeof(t_token)))
	{
		perror("tokenize");
		return (ERROR);
	}
	while (lexer.state != END_OF_INPUT)
	{
		if (g_lex[lexer.state](&lexer, get_type(lexer.str + lexer.idx)))
			continue ;
		perror("tokenize");
		dynarr_delete(tokens);
		return (ERROR);
	}
	warn_unclosed_quote(&lexer);
	return (SUCCESS);
}

static bool	delimit_token(t_lexer *lexer)
{
	t_token	new_token;

	if (lexer->state == NO_TYPE)
		lexer->state = END_OF_INPUT;
	new_token.type = lexer->state;
	new_token.flags = lexer->flags;
	new_token.start = lexer->token_start;
	new_token.end = lexer->idx - 1;
	if (new_token.type == OPERATOR)
		id_operator(lexer->str, &new_token);
	if (is_redir(new_token.type))
		lexer->flags |= REDIR_WORD;
	else if (!flag(lexer->flags, APPEND_NEXT))
		lexer->flags &= ~REDIR_WORD;
	return (dynarr_addone(lexer->tokens, &new_token));
}

bool	switch_state(t_lexer *lexer, t_char_type new_state)
{
	bool	rv;

	if (new_state == DEFAULT || new_state == EOF_S)
		rv = delimit_token(lexer);
	else
		rv = true;
	lexer->state = g_types[new_state];
	lexer->flags &= ~(APPEND_NEXT | IS_QUOTED | HAS_VAR | HAS_GLOB);
	lexer->token_start = lexer->idx;
	return (rv);
}

bool	consume_char(t_lexer *lexer)
{
	lexer->idx++;
	return (true);
}
