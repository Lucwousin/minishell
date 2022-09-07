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
# include <ms_types.h>
# include <stdlib.h>
# include <sys/types.h>

/*
 * The definition of an executor function.
 * Takes a pointer to the node, and a boolean signifying we're in a fork.
 */
typedef uint8_t	(*t_executor)(t_ast_node *, bool);

/*
 * A pipeline data structure. Used in execute_pipeline
 *
 * `orig' are copies of the original stdin and stdout
 * `pipe' is a place to create pipes
 * `idx'  is the index of the current node we're laying pipe for
 * `len'  is the total number of nodes inside this pipeline
 * `pids' is where the process id of every piped node is stored
 */
typedef struct s_pipeline {
	int32_t	orig[2];
	int32_t	pipe[2];
	size_t	idx;
	size_t	len;
	pid_t	*pids;
}	t_pipeline;

/**
 * Execute an AST node. Forwards execution on to the correct function.
 *
 * @param node[in] The node to execute
 * @param must_exit[in] True if we're in a fork, and we should exit instead of
 * return. If this is true, we don't need to fork the child process again.
 *
 * @return EXIT_SUCCESS if execution should continue, EXIT_FAILURE if not
 */
uint8_t	execute_node(t_ast_node *node, bool must_exit);

uint8_t	execute_subshell(t_ast_node *node, bool must_exit);
uint8_t	execute_command(t_ast_node *node, bool must_exit);
uint8_t	execute_pipeline(t_ast_node *node, bool must_exit);
uint8_t	execute_logic(t_ast_node *node, bool must_exit);

/**
 * Execute an executable binary. Searches the PATH for the binary, or tries to
 * execute the binary if the command name contains a `/'
 */
void	execute_binary(char **argv);

/**
 * Wait for a child process, and set the exit global to the exit status.
 *
 * @param pid[in] The process ID to wait on
 *
 * @return EXIT_FAILURE if an error occurred, EXIT_SUCCESS on success
 */
uint8_t	wait_for(pid_t pid, uint8_t *exit);

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
uint8_t	wait_pids(pid_t *pids, size_t len);

/**
 * Fork the current process, and wait for the child.
 *
 * @param status[out] A pointer to store the exit status (like wait_for returns)
 *
 * @return `false' in the child process
 *         `true' in the parent process and on error
 */
bool	fork_and_wait(uint8_t	*status);

/**
 * Fork the current process.
 *
 * @param pid_p[out] A pointer to store the pid of the child process
 * @return false if fork returned -1
 */
bool	do_fork(pid_t *pid_p);

/**
 * Duplicate the file descriptors STDIN/STDOUT are currently pointing at.
 * 
 * @return `true' if everything went ok, `false' if an error occurred
 */
bool	dup_stdio(int32_t dst[2]);

/**
 * Make the STDIN/STDOUT file descriptors point to the same files as the ones
 * in `fds'
 */
bool	redir_stdio(int32_t fds[2]);

bool	expand_variables(t_wordlist *cur, t_dynarr *buf);
bool	split_words(t_wordlist *cur);
bool	expand_filenames(t_wordlist *cur);
bool	remove_quotes(t_wordlist *cur, t_dynarr *buf);

/**
 * If must_exit is true, exit with the last exit code. If must_exit is false,
 * return status.
 */
static inline uint8_t	try_exit(uint8_t status, bool must_exit)
{
	if (must_exit)
		exit(g_env.exit);
	else
		return (status);
}

#endif
