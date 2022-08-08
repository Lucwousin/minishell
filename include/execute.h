/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/27 14:48:14 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/27 14:48:14 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include <minishell.h>
# include <parse.h>
# include <stdlib.h>
# include <stdnoreturn.h>

typedef uint8_t	(*t_executor)(union u_node *, bool);

typedef struct s_pipeline {
	int32_t	orig[2];
	int32_t	pipe[2];
	size_t	idx;
	size_t	len;
	pid_t	*pids;
}	t_pipeline;

/**
 * Execute the abstract syntax tree.
 *
 * @param root_node[in] The root node of the tree
 * 
 * @return EXIT_SUCCESS if execution succeeded, EXIT_FAILURE if not
 */
uint8_t			execute(t_ast_node *root_node);

/**
 * Execute an AST node. Forwards execution on to the correct function.
 *
 * @param node[in] The node to execute
 * @param must_exit[in] True if we're in a fork, and we should exit instead of
 * return. If this is true, we don't need to fork the child process again.
 *
 * @return EXIT_SUCCESS if execution should continue, EXIT_FAILURE if not
 */
uint8_t			execute_node(t_ast_node *node, bool must_exit);

uint8_t			execute_subshell(t_paren_node *node, bool must_exit);
uint8_t			execute_command(t_cmd_node *cmd, bool must_exit);
uint8_t			execute_pipeline(t_pipe_node *node, bool must_exit);
uint8_t			execute_logic(t_logic_node *node, bool must_exit);

/**
 * Execute an executable binary. Searches the PATH for the binary, or tries to
 * execute the binary if the command name contains a `/'
 */
noreturn void	execute_binary(t_cmd_node *cmd);

/**
 * Wait for a child process, and set the exit global to the exit status.
 *
 * @param pid[in] The process ID to wait on
 *
 * @return EXIT_FAILURE if an error occurred, EXIT_SUCCESS on success
 */
uint8_t			wait_for(pid_t pid);

/**
 * Wait for multiple child processes, and set the exit global to the last
 * process's exit status. Stops waiting when a invalid pid is reached, in which
 * case EXIT_FAILURE is returned (and set) and an error message is printed.
 *
 * @param pids[in] The process IDs to wait on
 * @param len[in] The size of `pids'
 *
 * @return EXIT_FAILURE if an error occurred, EXIT_SUCCESS on success
 */
uint8_t			wait_pids(pid_t *pids, size_t len);

/**
 * Fork the current process, and wait for the child.
 *
 * @param status[out] A pointer to store the exit status (like wait_for returns)
 *
 * @return `false' in the child process
 *         `true' in the parent process and on error
 */
bool			fork_and_wait(uint8_t	*status);

/**
 * Duplicate the file descriptors STDIN/STDOUT are currently pointing at.
 * 
 * @return `true' if everything went ok, `false' if an error occurred
 */
bool			dup_stdio(int32_t dst[2]);

/**
 * Make the STDIN/STDOUT file descriptors point to the same files as the ones
 * in `fds'
 */
bool			redir_stdio(int32_t fds[2]);

/**
 * If must_exit is true, exit with the last exit code. If must_exit is false,
 * return status.
 */
static inline uint8_t	try_exit(uint8_t status, bool must_exit)
{
	if (must_exit)
		exit(g_globals.exit);
	else
		return (status);
}

#endif