/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/15 13:48:13 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/15 13:48:13 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include <dynarr.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

typedef enum e_tokentype {
	END_OF_INPUT = 0,
	WHITESPACE,
	WORD,
	VARIABLE,
	SQUOTE,
	DQUOTE,
	PAR_OPEN,
	PAR_CLOSE,
	OPERATOR,
	RED_IN,
	RED_HD,
	RED_HD_Q,
	RED_OUT,
	RED_APP,
	OR,
	AND,
	PIPE,
	GLOB
}	t_tokentype;

typedef struct s_token {
	t_tokentype	type;
	size_t		start;
	size_t		end;
}	t_token;

typedef struct s_expanded_token {
	t_tokentype	type;
	char		*str;
}	t_exp_tok;

/**
 * Tokenize the user input. Splits everything up into tokens (lexemes)
 * 
 * @param tokens[out] A dynamic array all the tokens will be added to.
 * @param cmd[in] The user input
 * 
 * @return true if nothing went wrong, false if an allocation failed
 */
uint8_t	tokenize(t_dynarr *tokens, const char *cmd);

/**
 * Remove all unnecessary whitespace and unclosed quotes
 * 
 * @param tokens[in/out] the tokens to evaluate
 *
 * @return true if nothing went wrong, false if an allocation failed
 */
uint8_t	evaluate(t_dynarr *tokens);

/**
 * Join all adjacent words, remove quotes, expand variables, combine redirects
 * with their arguments.
 * 
 * @param tokens[in] the tokens to expand
 * @param cmd[in]
 * @param exp_tokens[out] the output array of expanded tokens
 * 
 * @return true if nothing went wrong, false in the case of a syntax error or
 * allocation failure
 */
uint8_t	preparse(const char *cmd, t_dynarr *tokens, t_dynarr *exp_tokens);

#endif //INPUT_H
