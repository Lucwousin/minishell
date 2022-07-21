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

# include <dynarr.h>
#include "input.h"

# define PIPE_INIT_SIZE		8
# define ARGV_INIT_SIZE		8
# define REDIR_INIT_SIZE	4

typedef enum e_ast_type {
	COMMAND,
	LOGICAL_EXPRESSION,
	PARENTHESIS,
	PIPELINE,
}	t_ast_type;

typedef struct s_ast_node	t_ast_node;

typedef struct s_cmd_node	t_cmd_node;
typedef struct s_logic_node	t_logic_node;
typedef struct s_paren_node	t_paren_node;
typedef struct s_pipe_node	t_pipe_node;

typedef struct s_redirection {
	t_tokentype	type;
	char		*str;
}	t_redir;

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

typedef struct s_parser {
	size_t		idx;
	t_dynarr	*tokens;
}	t_parser;

t_ast_node	*build_ast(t_dynarr *tokens);

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
 * Parse a node nodes. This is not really a nodes, because it only ever contains
 * one type of node. The root node is a node nodes, and so are parenthesis.
 *
 * A node nodes can contain any one of the following node types:
 *   * Another node nodes (parenthesis)
 *   * A command
 *   * A pipeline
 *   * A logical operator
 *
 * If the node nodes is not the root node, it will be terminated by END_OF_INPUT
 * If it was not the root node (parenthesis) it will be terminated by PAR_CLOSE
 * A syntax error will print the error message, and set *dst to NULL
 */
t_tokentype	parse_nodelist(t_parser *parser, t_ast_node **dst, bool paren);
/**
 * Parse a command node. Command nodes have a nodes of redirections, and a nodes
 * of arguments.
 *
 * The definition of a command in tokens, is n * (WORD || REDIR)
 * Commands are terminated by PAR_CLOSE, a logic operator, or a pipe operator
 * If the command was terminated by something else (parenthesis, lol), a syntax
 * error will be printed and *dst will be set to NULL.
 */
t_tokentype	parse_command(t_parser *parser, t_ast_node **dst);
/**
 * Parse a pipeline. Pipelines are lists of commands.
 *
 * Because the pipe operator is to the right of the first command, *dst will
 * already contain a command node. This will be replaced with a pointer to the
 * new pipeline node, which has the command in its' nodes.
 */
t_tokentype	parse_pipeline(t_parser *parser, t_ast_node **dst);
t_tokentype	parse_logic(t_parser *parser, t_ast_node **dst, t_tokentype type);

t_ast_node	*init_cmd_node(void);
t_ast_node	*init_paren_node(void);
t_ast_node	*init_pipe_node(t_ast_node *first);
t_ast_node	*init_logic_node(t_tokentype type, t_ast_node *left);
void		destroy_node(t_ast_node **nodep);

#endif //PARSE_H
