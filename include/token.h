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
	t_dynarr	*output;
	size_t		idx;
	t_exp_tok	cur;
	bool		in_q[2];
	int32_t		exit_status;
}	t_preparser;

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
bool		switch_state(t_lexer *lexer, t_lex_state new_state);
/**
 * Set the scanners' state. This function basically does the same thing as
 * switch_state, but without delimiting the current token. This is useful to
 * switch to a new state halfway through parsing something.
 * 
 * @param lexer[in/out] the scanner 
 * @param new_state[in] the new state to switch to
 * @return Always returns true
 */
bool		set_state(t_lexer *lexer, t_lex_state new_state);

/*
 * All functions starting with `lex_` are functions handling next characters
 * when inside a certain token. If the next character type can not be accepted
 * by the state, the state is switched back to DEFAULT.
 * 
 * These functions return true if everything went as planned, false if any
 * allocation failed when delimiting a token.
 */
bool		lex_operator(t_lexer *lexer, t_char_type type);
bool		lex_variable(t_lexer *lexer, t_char_type type);
bool		lex_simple(t_lexer *lexer, t_char_type type);
bool		lex_simple_single(t_lexer *lexer, t_char_type type);

/*				PREPARSER			*/
/**
 * Expand a variable. Adds a string representation of the variable to buf.
 * This contains handling for variables with spaces in the values:
 *    If a variable contains whitespace and is not quoted, the spaces will get
 *    removed and every word will get added to the expanded token list
 *    individually.
 * 
 * @param pp[in/out] The preparsers' instance
 * @param tok[in] The variable token to expand
 * @param buf[in/out] The buffer the value of the variable will get added to
 *
 * @return true if everything went as planned, false if any allocation failed
 */
bool		expand_var(t_preparser *pp, t_token *tok, t_dynarr *buf);

/*				UTILS				*/
/**
 * Get the substring of this token.
 * 
 * @param cmd[in] The user input 
 * @param buf[in/out] The buffer to add the substring to
 * @param token[in] The token we want to get the substring of
 * 
 * @return true if everything went as planend, false if any allocation failed
 */
bool		add_token(const char *cmd, t_dynarr *buf, t_token *token);
/**
 * Toggles the boolean at the index of the quote type `type` represents
 * 
 * @param in_quote[in/out] A boolean array, with index 0 representing quoted
 *   quoted status for single quotes and index 1 representing double quotes
 * @param type[in] The token type we want to toggle.
 * 
 * @return true if the quoted status actually changed, false if it did not.
 *   This could happen if type was not a quote type, or if we were already in
 *   the other quote type.
 */
bool		toggle_quote(bool in_quote[2], t_tokentype type);
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
