/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ms_types.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/31 13:40:41 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/31 13:40:41 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_TYPES_H
# define MS_TYPES_H

# include <dynarr.h>
# include <stdint.h>

# define SUCCESS	0
# define ERROR		1
# define SYNTAX		2

typedef enum e_tokentype {
	NO_TYPE,
	WHITESPACE,
	WORD,
	VARIABLE,
	SQUOTE,
	DQUOTE,
	OPERATOR,
	RED_IN,
	RED_HD,
	RED_HD_Q,
	RED_OUT,
	RED_APP,
	PAR_OPEN,
	PAR_CLOSE,
	OR,
	AND,
	PIPE,
	END_OF_INPUT
}	t_tokentype;

typedef enum e_ast_type {
	COMMAND,
	LOGIC,
	PARENTHESIS,
	PIPELINE,
}	t_ast_type;

typedef enum e_flag {
	S_QUOTED = 1,
	D_QUOTED = 2,
	APPEND_NEXT = 4,
	HAS_VAR = 8,
	HAS_GLOB = 16,
	REDIR_WORD = 32,
	UNCLOSED_QUOTE = 64,
	IS_QUOTED = S_QUOTED | D_QUOTED,
}	t_flag;

typedef struct s_token		t_token;
struct s_token {
	t_tokentype	type;
	t_flag		flags;
	size_t		start;
	size_t		end;
};

typedef struct s_wordlist	t_wordlist;
struct s_wordlist {
	t_wordlist	*next;
	char		*word;
	t_flag		flags;
};

typedef struct s_cmd_node	t_cmd_node;
typedef struct s_logic_node	t_logic_node;
typedef struct s_subsh_node	t_subsh_node;
typedef struct s_pipe_node	t_pipe_node;

typedef struct s_ast_node	t_ast_node;
struct s_ast_node {
	t_ast_type	type;
	union {
		struct s_cmd_node {
			t_dynarr	redirs;
			t_wordlist	args;
		}	command;
		struct s_logic_node {
			t_tokentype	type;
			t_ast_node	*l;
			t_ast_node	*r;
		}	logic;
		struct s_subsh_node {
			t_ast_node	*contents;
		}	subsh;
		struct s_pipe_node {
			t_dynarr	nodes;
		}	pipe;
	};
};

bool		flag(t_flag flags, t_flag flag);
bool		is_redir(t_tokentype type);

t_wordlist	*new_wordlist(char *word, uint8_t flags);
t_wordlist	*new_wordlist_token(const char *cmd, const t_token *token);
uint8_t		destroy_wordlist(t_wordlist *cur);
bool		combine(t_wordlist *start, t_wordlist *end);

#endif