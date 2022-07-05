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
# define SUB_INIT_SIZE			4
# define BLANK_CHARS			" \t"
# define OPERATOR_CHARS			"|<>"
# define QUOTE_CHARS			"'\""
# define SINGLE_QUOTE			'\''
# define DOUBLE_QUOTE			'"'
# define VAR_CHAR				'$'
# define PIPE_STR				"|"
# define RED_HD_STR				"<<"
# define RED_IN_STR				"<"
# define RED_APPEND_STR			">>"
# define RED_OUT_STR			">"

# include <dynarr.h>

/**
 * A token type
 *
 * END_OF_INPUT signifies the end of the input
 *
 * WORD is for everything that's not an operator type.
 *     VARIABLE is a subtoken type for a word (or quote), signifying a variable
 *     QUOTE is a subtoken type, signifying a quoted part of a word. This can
 *         contain variables.
 *
 * OPERATOR is for pipes and redirections. All token types with value greater
 * than this one are operator types:
 *     PIPE is an operator type signifying a pipe "|"
 *     RED_IN is an operator type signifying a input redirection "<"
 *     RED_HD is an operator type signifying a here_document "<<"
 *         This includes the delimiter, to make handling quoted delimiters
 *         possible
 *     RED_OUT is an operator type signifying a output redirection ">"
 *     RED_APP is an operator type signifying appending redirection ">>"
 */
typedef enum e_tokentype {
	END_OF_INPUT,
	WORD,
	VARIABLE,
	QUOTE,
	OPERATOR,
	PIPE,
	RED_IN,
	RED_HD,
	RED_OUT,
	RED_APP
}	t_tokentype;

/**
 * A struct containing information about a token in the input.
 * token is the token type
 * start is the start index of the token inside the input string (inclusive)
 * end is the end index of the token inside the input string (also inclusive)
 *
 * "   this_is_a_word    " for example:
 * token = WORD
 * start = 3 (because input[3] == 't')
 * end = 16 (because input[16] == 'd')
 * 
 * sub is a dynamic array containing the sub-tokens for this token
 * (only initialized for WORD and QUOTE type tokens)
 */
typedef struct s_token {
	t_tokentype	token;
	size_t		start;
	size_t		end;
	t_dynarr	sub;
}	t_token;

/**
 * A token after expansion. All variables are expanded to their values, quotes
 * are removed. Str is a freshly allocated string with the characters that make
 * up this token.
 */
typedef struct s_expanded_token {
	t_tokentype	type;
	char		*str;
}	t_exp_token;

/**
 * A simple struct used as an argument for expand_token (expander.c)
 * 
 * _cmd_ is the user input, _index_ is the current index in _expanded_
 */
typedef struct s_expander_argument {
	t_exp_token	*expanded;
	const char	*cmd;
	size_t		index;
}	t_exp_arg;

/**
 * Tokenize the user input. Splits everything up into words and operators.
 * Words can have sub-tokens (quotes and variables). Quotes also can have subs
 * (variables).
 * 
 * @param tokens[out] A dynamic array all the tokens will be added to.
 * @param cmd[in] The user input
 */
void		tokenize(t_dynarr *tokens, const char *cmd);

/**
 * Match a token!
 * 
 * @param cmd[in] The user input 
 * @param idx[in/out] The current index in the user input
 * @param token[in/out] The current token we're creating. Starts off as EOF.
 */
void		match_token(const char *cmd, size_t *idx, t_token *token);

/**
 * Match an operator!
 * 
 * @param cmd[in] The user input
 * @param idx[in/out] the current index in the user input
 * @param token[in/out] The current token we're creating (set to OPERATOR)
 */
void		match_operator(const char *cmd, size_t *idx, t_token *token);

/**
 * Match a word!
 * 
 * @param cmd[in] The user input
 * @param idx[in/out] the current index in the user input
 * @param token[in/out] The current token we're creating (set to WORD)
 * 
 * This function recursively calls match_token, until the current token is
 * delimited by the end of input, whitespace, or an operator
 * If there's a quoted part in this word, it is added as a subtoken to the word.
 * If there's a variable in this word, or in a quote, it is added as a subtoken
 * to the token it is part of (so a quote, or the word)
 */
void		match_word(const char *cmd, size_t *idx, t_token *token);

/**
 * Expand all tokens!
 * 
 * @param tokens[in] All tokens returned from tokenize 
 * @param cmd[in] The user input
 * 
 * @return An array of all expanded tokens, delimited by a expanded token with
 * type END_OF_INPUT.
 */
t_exp_token	*expand(t_dynarr *tokens, const char *cmd);

#endif
