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

# include <ms_types.h>

typedef enum e_in_handler_state {
	TOKENIZE,
	PARSE,
	EXECUTE,
	DONE
}	t_hstate;

typedef struct s_in_handler	t_in_handler;
typedef void				(*t_h_hook)(t_in_handler *);

struct s_in_handler {
	const char	*input;
	t_hstate	state;
	t_dynarr	tokens;
	t_ast_node	*root_node;
	t_h_hook	hooks[DONE];
};

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
uint8_t		tokenize(t_dynarr *tokens, const char *cmd);

/**
 * Parse all expanded tokens in tokens to an AST tree representing the command
 * line we have to execute.
 *
 * @param cmd[in] The user input
 * @param tokens[in] The expanded tokens, made by preparse
 * @param dst[out] A pointer where the root node will be stored.
 *
 * @return EXIT_SUCCESS if everything went ok, EXIT_FAILURE on error
 */
uint8_t		build_ast(const char *cmd, t_dynarr *tokens, t_ast_node **dst);

/**
 * Execute the abstract syntax tree.
 *
 * @param root_node[in] The root node of the tree
 *
 * @return EXIT_SUCCESS if execution succeeded, EXIT_FAILURE if not
 */
uint8_t		execute(t_ast_node *root_node);

/**
 * Initialize the handler. Every place where dynamically allocated memory
 * will be stored gets set to NULL, state gets set to TOKENIZE, and the
 * hooks array will be filled with NULL.
 */
void		init_handler(t_in_handler *handler, const char *input);

/**
 * Clean the handler. All dynamically allocated memory will get cleaned.
 *
 * Returns status
 */
uint8_t		clean_handler(t_in_handler *handler, uint8_t status);

/**
 * Handle input up until a certain target. Used for testing.
 *
 * Does not clean up like handle_input does, use clean_handler for that.
 *
 * This allows you to add hooks to the handler, called before changing states.
 * If you change the state to DONE (or > tgt) in a hook, the handling will stop.
 */
uint8_t		handle_input_target(t_in_handler *handler, t_hstate tgt);

#endif //INPUT_H
