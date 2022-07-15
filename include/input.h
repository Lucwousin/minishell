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
	PIPE
}	t_tokentype;

/**
 * Tokenize the user input. Splits everything up into tokens (lexemes)
 * 
 * @param tokens[out] A dynamic array all the tokens will be added to.
 * @param cmd[in] The user input
 * 
 * @return true if nothing went wrong, false if an allocation failed
 */
bool		tokenize(t_dynarr *tokens, const char *cmd);

/**
 * Remove all unnecessary whitespace and unclosed quotes
 * 
 * @param tokens[in/out]
 *
 * @return 
 */
bool		evaluate(t_dynarr *tokens);

#endif //INPUT_H
