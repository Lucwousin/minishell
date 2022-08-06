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

# include <parse.h>
# include <sys/types.h>
# include <stdlib.h>

typedef uint8_t	(*t_executor)(union u_node *, bool);

/**
 * Execute the abstract syntax tree.
 *
 * @param root_node[in] The root node of the tree
 */
void	execute(t_ast_node *root_node);

/**
 * Execute an AST node. Forwards execution on to the correct function.
 *
 * @param node[in] The node to execute
 * @param can_exit[in] True if the current process can be used to execve
 *
 * @return The exit status of this node // TODO: Error return only otherwise 0
 */
uint8_t	execute_node(t_ast_node *node, bool can_exit);

uint8_t	execute_subshell(t_paren_node *node, bool can_exit);
uint8_t	execute_command(t_cmd_node *cmd, bool can_exit);
void	execute_binary(t_cmd_node *cmd);
uint8_t	execute_logic(t_logic_node *node, bool can_exit);

/**
 * Wait for a child process.
 *
 * @param pid[in] The process ID to wait on
 *
 * @return If the process exited, the exit status.
 *         If the process was terminated, 128 + the signal number
 */
uint8_t	wait_for(pid_t pid);

/**
 * Fork the current process, and wait for the child.
 *
 * @param status[out] A pointer to store the exit status (like wait_for returns)
 *
 * @return `false' in the child process
 *         `true' in the parent process and on error
 */
bool	fork_and_wait(uint8_t *status);

static inline uint8_t	finish_leaf(uint8_t status, bool can_exit)
{
	if (can_exit)
		exit(status);
	else
		return (status);
}

#endif
