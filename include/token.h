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

# define TOKENS_INIT_SIZE	32
# define BLANK_CHARS		" \t"
# define OPERATOR_CHARS		"|<>"
# define QUOTE_CHARS		"'\""

# include <dynarr.h>

/**
 * A token type
 *
 * OPERATOR is for pipes and redirections
 * WORD is for everything else
 * END_OF_INPUT signifies the end of the input (duh)
 */
typedef enum e_token {
	END_OF_INPUT,
	OPERATOR,
	WORD
}	t_token;

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
 */
typedef struct s_codepoint {
	t_token	token;
	size_t	start;
	size_t	end;
}	t_codepoint;

t_dynarr	tokenize(const char *cmd);

void		match_token(const char *cmd, size_t *idx, t_codepoint *token);

void		resize_tokens(t_dynarr *data, size_t new_len);

#endif
