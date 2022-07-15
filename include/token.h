/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 14:00:50 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/22 14:00:50 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# define TOKENS_INIT_SIZE		32

# define EOF_CHARS				"\n"
# define BLANK_CHARS			" \t"
# define OPERATOR_CHARS			"|<>"
# define SINGLE_QUOTE			'\''
# define DOUBLE_QUOTE			'"'
# define VAR_CHAR				'$'
# define PAR_OPEN_CHAR			'('
# define PAR_CLOSE_CHAR			')'

# define TOKEN_S_QUOTED			128
# define TOKEN_D_QUOTED			256

# include <dynarr.h>
# include <input.h>

typedef enum e_lexer_state {
	DEFAULT,
	WHITE_S,
	OPERATOR_S,
	WORD_S,
	SQUOTE_S,
	DQUOTE_S,
	VAR_S,
	PAR_OPEN_S,
	PAR_CLOSE_S,
	EOF_S,
}	t_lex_state;

typedef t_lex_state	t_char_type;

typedef struct s_token {
	t_tokentype	token;
	size_t		start;
	size_t		end;
}	t_token;

typedef struct s_lexer {
	t_lex_state	state;
	t_dynarr	*tokens;
	t_token		current_token;
	const char	*str;
	size_t		idx;
}	t_lexer;

typedef bool		(*t_lexerfunc)(t_lexer *, t_char_type);

bool		lex_operator(t_lexer *lexer, t_char_type type);
bool		lex_variable(t_lexer *lexer, t_char_type type);
bool		lex_simple(t_lexer *lexer, t_char_type type);
bool		lex_simple_single(t_lexer *lexer, t_char_type type);

bool		consume_char(t_lexer *lexer);
bool		switch_state(t_lexer *lexer, t_lex_state new_state);

static inline bool	add_token(const char *cmd, t_dynarr *buf, t_token *token)
{
	size_t	len;

	len = token->end - token->start + 1;
	return (dynarr_add(buf, cmd + token->start, len));
}

#endif
