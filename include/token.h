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

typedef struct s_lexer {
	t_lex_state	state;
	t_dynarr	*tokens;
	t_token		current_token;
	const char	*str;
	size_t		idx;
}	t_lexer;

typedef struct s_preparser {
	const char	*cmd;
	t_dynarr	*tokens;
	size_t		idx;
	t_exp_tok	cur;
	bool		in_q[2];
}	t_preparser;

typedef bool		(*t_lexerfunc)(t_lexer *, t_char_type);

bool		lex_operator(t_lexer *lexer, t_char_type type);
bool		lex_variable(t_lexer *lexer, t_char_type type);
bool		lex_simple(t_lexer *lexer, t_char_type type);
bool		lex_simple_single(t_lexer *lexer, t_char_type type);

bool		consume_char(t_lexer *lexer);
bool		switch_state(t_lexer *lexer, t_lex_state new_state);

bool		add_token(const char *cmd, t_dynarr *buf, t_token *token);
bool		toggle_quote(bool in_quote[2], t_tokentype type);
void		id_operator(const char *cmd, t_token *token, t_token *next);
t_char_type	get_type(const char *c);

#endif
