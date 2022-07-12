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

#ifndef PARSER_H
# define PARSER_H

# include <dynarr.h>

# define AST_INIT_SIZE	32

typedef enum e_ast_type {
	COMMAND,
	LOGICAL_EXPRESSION,
	REDIRECT,
	PIPELINE
}	t_ast_type;

typedef enum e_red_type {
	REDIR_IN,
	REDIR_HD,
	REDIR_HD_QUOTED,
	REDIR_OUT,
	REDIR_APPEND
}	t_red_type;

typedef struct s_ast_node {
	t_ast_type	type;
	void		*data;
}	t_ast_node;

typedef struct s_cmd_node		t_cmd_node;
typedef struct s_red_node		t_red_node;
typedef struct s_pipe_node		t_pipe_node;
typedef struct s_logic_node		t_logic_node;

struct s_red_node {
	t_red_type	type;
	char		*file;
};

struct s_cmd_node {
	t_dynarr	**argv;
	t_red_node	redirs[2];
};

struct s_pipe_node {
	t_dynarr	cmds;
};

struct s_logic_node {
	void	*l;
	void	*r;
};

bool	build_ast(const char *cmd, t_dynarr *tokens, t_dynarr *trunk);

#endif //PARSER_H
