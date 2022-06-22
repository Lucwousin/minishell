/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: user <user@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 14:00:50 by user          #+#    #+#                 */
/*   Updated: 2022/06/22 14:00:50 by user          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# define TOKENS_INIT_SIZE 32
# define BLANK_CHARS " \t"
# define OPERATOR_CHARS "|<>"
# define QUOTE_CHARS "'\""

# include <stdbool.h>
# include <stddef.h>

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

typedef struct s_tokens {
	t_codepoint	*tokens;
	size_t		tokens_size;
}	t_tokens;

t_codepoint	*tokenize(const char *cmd);

void		match_token(const char *cmd, size_t *idx, t_codepoint *token);

#endif
