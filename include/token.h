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

# include <dynarr.h>

/**
 * A token type
 *
 * OPERATOR is for pipes and redirections
 * WORD is for everything else
 * END_OF_INPUT signifies the end of the input (duh)
 * VARIABLE is a subtoken type, signifying a variable
 * QUOTE is a subtoken type, signifying a quoted part
 */
typedef enum e_tokentype {
	END_OF_INPUT,
	OPERATOR,
	WORD,
	VARIABLE,
	QUOTE
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

typedef struct s_expanded_token {
	t_tokentype	type;
	char		*str;
}	t_exp_token;

void		tokenize(t_dynarr *tokens, const char *cmd);
void		match_token(const char *cmd, size_t *idx, t_token *token);

t_exp_token	*expand(t_dynarr *tokens, const char *cmd);

#endif
