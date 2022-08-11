/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/12 16:08:54 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/12 16:08:54 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include <input.h>

# define PIPE_INIT_SIZE		8
# define ARGV_INIT_SIZE		8
# define REDIR_INIT_SIZE	4

# define SUCCESS			0
# define ERROR				1
# define SYNTAX				2

typedef struct s_parser {
	size_t		idx;
	t_dynarr	*tokens;
}	t_parser;

/**
 * Parse a single AST node. Explanations for possibilities are below.
 *
 * If there was a syntax error, the parser functions will set *dst to NULL.
 *
 * Token type  -> parse function:
 *
 *    WORD     -> parse_command
 *    VARIABLE -> parse_command
 *    RED_*    -> parse_command
 *    PAR_OPEN -> parse_nodelist
 *    OR       -> parse_logic
 *    AND      -> parse_logic
 *    PIPE     -> parse_pipeline
 *
 * @param parser[in/out] The state of the parser
 * @param type[in] The token type to create a node for
 * @param dst[out] A pointer to where a pointer the new node should be stored
 *
 * @return The type of the first token that could not be appended to the node
 */
t_tokentype	parse_node(t_parser *parser, t_tokentype type, t_ast_node **dst);
/**
 * Parse a node list. This is not really a list, because it only ever contains
 * one type of node. The root node is a node list, and so are parenthesis.
 *
 * A node nodes can contain any one of the following node types:
 *   * Another node list (parenthesis)
 *   * A command
 *   * A pipeline
 *   * A logical operator
 *
 * If the node list is the root node, it will be terminated by END_OF_INPUT
 * If it was not the root node (parenthesis) it will be terminated by PAR_CLOSE
 * A syntax error will print the error message, and set *dst to NULL
 */
t_tokentype	parse_nodelist(t_parser *parser, t_ast_node **dst, bool paren);
/**
 * Parse a command node. Command nodes have a list of redirections, and a list
 * of arguments.
 *
 * The definition of a command in tokens, is n * (WORD || REDIR)
 * Commands are terminated by PAR_CLOSE, a logic operator, or a pipe operator
 * If the command was terminated by something else (parenthesis, lol), a syntax
 * error will be printed and *dst will be set to NULL.
 */
t_tokentype	parse_command(t_parser *parser, t_ast_node **dst);
/**
 * Parse a pipeline. Pipelines are lists of ast nodes that are NOT pipes.
 *
 * Because the pipe operator is to the right of the first node, *dst will
 * already contain a node. This will be replaced with a pointer to the
 * new pipeline node, which has the node in its' nodes.
 */
t_tokentype	parse_pipeline(t_parser *parser, t_ast_node **dst);
/**
 * Parse a logical expression. Logical expression contain a lhs and a rhs sub
 * node.
 * 
 * Because the logical expression is to the right of the first node, *dst will
 * already contain a node. This will be replaced with a pointer to the new
 * logical expression node, which contains the node as it's left hand side.
 */
t_tokentype	parse_logic(t_parser *parser, t_ast_node **dst, t_tokentype type);

/**
 * Functions to allocate and initialize new AST nodes
 */
t_ast_node	*init_cmd_node(void);
t_ast_node	*init_paren_node(void);
t_ast_node	*init_pipe_node(t_ast_node *first);
t_ast_node	*init_logic_node(t_tokentype type, t_ast_node *left);

/**
 * Recursively destroy a node, and all it's children
 */
void		destroy_node(t_ast_node **nodep);

/**
 * Set the exit status to `status', prints an error message (if necessary),
 * destroys `dst', and returns `-1' (The error return value for all `parse_*'
 * functions.
 * 
 * @param node[in/out] The node we want to destroy (or NULL)
 * @param where[in] An optional error message, only gets printed on ERROR
 * @param status[in] The exit to set.
 * @return -1
 */
uint8_t		error_status(t_ast_node **node, char *where, uint8_t status);

#endif //PARSE_H
