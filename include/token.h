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
# define OPERATOR_CHARS			"&|<>()"
# define SINGLE_QUOTE			'\''
# define DOUBLE_QUOTE			'"'
# define VAR_CHAR				'$'
# define PAR_OPEN_CHAR			'('
# define PAR_CLOSE_CHAR			')'
# define WILDCARD_CHAR			'*'

# include <input.h>

typedef enum e_lexer_state {
	DEFAULT,
	WHITE_S,
	OPERATOR_S,
	WORD_S,
	SQUOTE_S,
	DQUOTE_S,
	VAR_S,
	WILDCARD_S,
	EOF_S,
}	t_char_type;

typedef struct s_lexer {
	t_tokentype	state;
	uint8_t		flags;
	size_t		token_start;
	t_dynarr	*tokens;
	const char	*str;
	size_t		idx;
}	t_lexer;

typedef bool		(*t_lexerfunc)(t_lexer *, t_char_type);

/*				SCANNER				*/
/**
 * Make the scanner consume a character (increment index)
 * 
 * @param lexer[in/out] the scanner
 * @return true, every single time
 */
bool		consume_char(t_lexer *lexer);
/**
 * Make the scanner switch to a new state. If the new state is the accepting
 * or finished state (DEFAULT OR EOF_S), delimit the current token.
 * 
 * @param lexer[in/out] the scanner
 * @param new_state[in] the new state to switch to
 * @return true if everything went as planned, false if any allocation failed
 *   when delimiting the token
 */
bool		switch_state(t_lexer *lexer, t_char_type new_state);

/*
 * All functions starting with `lex_` are functions handling next characters
 * when inside a certain token. If the next character type can not be accepted
 * by the state, the state is switched back to DEFAULT.
 * 
 * These functions return true if everything went as planned, false if any
 * allocation failed when delimiting a token.
 */
bool		lex_operator(t_lexer *lexer, t_char_type type);
bool		lex_whitespace(t_lexer *lexer, t_char_type type);
bool		lex_word(t_lexer *lexer, t_char_type type);

/*				UTILS				*/
/**
 * Identify (and change) the operator for the OPERATOR type token `token`
 * 
 * @param cmd[in] The user input
 * @param token[in/out] The token we want to identify. This token should be of
 *   type OPERATOR!
 */
void		id_operator(const char *cmd, t_token *token);
/**
 * Get the character type for the character pointed to by `c`.
 * 
 * @param c[in] A pointer to a character. This is a pointer because it uses the
 *   next index to make sure we're dealing with a variable.
 *
 * @return The character type the character pointed to by `c` represents
 */
t_char_type	get_type(const char *c);

#endif
