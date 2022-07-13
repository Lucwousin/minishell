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

# define AST_INIT_SIZE	32

typedef enum e_ast_type {
	COMMAND,
	LOGICAL_EXPRESSION,
	PARENTHESIS,
	PIPELINE,
	REDIRECT,
}	t_ast_type;

typedef enum e_red_type {
	REDIR_IN,
	REDIR_HD,
	REDIR_HD_QUOTED,
	REDIR_OUT,
	REDIR_APPEND
}	t_red_type;

typedef union u_ast_node		t_ast_node;

typedef struct s_cmd_node		t_cmd_node;
typedef struct s_logic_node		t_logic_node;
typedef struct s_paren_node		t_paren_node;
typedef struct s_pipe_node		t_pipe_node;
typedef struct s_red_node		t_red_node;

union u_ast_node {
	t_ast_type	type;
	struct s_cmd_node {
		t_ast_type	ast_type;
		t_dynarr	argv;
		t_dynarr	redirs;
	}	command;
	struct s_logic_node {
		t_ast_type	ast_type;
		t_ast_node	*l;
		t_ast_node	*r;
	}	logic;
	struct s_paren_node {
		t_ast_type	ast_type;
		t_dynarr	contents;
	}	paren;
	struct s_pipe_node {
		t_ast_type	ast_type;
		t_dynarr	cmds;
	}	pipe;
	struct s_red_node {
		t_ast_type	ast_type;
		t_red_type	type;
		char		*file;
	}	redir;
};

bool	build_ast(const char *cmd, t_dynarr *tokens, t_dynarr *trunk);

#endif //PARSE_H
