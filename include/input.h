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
# include <stdint.h>

# define SUCCESS	0
# define FAILURE	1

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

typedef enum e_in_handler_state {
	TOKENIZE,
	EVALUATE,
	PREPARSE,
	PARSE,
	EXECUTE,
	DONE
}	t_hstate;

typedef enum e_ast_type {
	COMMAND,
	LOGIC,
	PARENTHESIS,
	PIPELINE,
}	t_ast_type;

typedef struct s_ast_node	t_ast_node;

typedef struct s_cmd_node	t_cmd_node;
typedef struct s_logic_node	t_logic_node;
typedef struct s_paren_node	t_paren_node;
typedef struct s_pipe_node	t_pipe_node;

struct s_ast_node {
	t_ast_type	type;
	union u_node {
		struct s_cmd_node {
			t_dynarr	argv;
			t_dynarr	redirs;
		}	command;
		struct s_logic_node {
			t_tokentype	type;
			t_ast_node	*l;
			t_ast_node	*r;
		}	logic;
		struct s_paren_node {
			t_ast_node	*contents;
		}	paren;
		struct s_pipe_node {
			t_dynarr	nodes;
		}	pipe;
	}	node;
};

typedef struct s_in_handler	t_in_handler;
typedef void	(*t_h_hook)(t_in_handler *);


struct s_in_handler {
	const char	*input;
	t_hstate	state;
	t_dynarr	tokens;
	t_dynarr	expanded_tokens;
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
 * Remove all unnecessary whitespace and unclosed quotes
 * 
 * @param tokens[in/out] the tokens to evaluate
 *
 * @return true if nothing went wrong, false if an allocation failed
 */
uint8_t		evaluate(t_dynarr *tokens);

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
uint8_t		preparse(const char *cmd, t_dynarr *tokens, t_dynarr *exp_tokens);

/**
 * Parse all expanded tokens in tokens to an AST tree representing the command
 * line we have to execute.
 *
 * @param tokens[in] The expanded tokens, made by preparse
 * @param dst[out] A pointer where the root node will be stored.
 *
 * @return EXIT_SUCCESS if everything went ok, EXIT_FAILURE on error
 */
uint8_t		build_ast(t_dynarr *tokens, t_ast_node **dst);

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
