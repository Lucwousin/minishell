/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/04 19:32:37 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/04 19:32:37 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <execute.h>

typedef enum e_builtin {
	NONE,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}	t_builtin;

/**
 * Check if the name (first argument) of a command corresponds to a builtin
 *
 * @param argv[in] The command argument array
 *
 * @return The builtin for this command, or NONE if it's not a builtin
 */
t_builtin	identify_command(char **argv);

/**
 * Execute a builtin and return the exit status.
 * (exit obviously does not return a status, unless there were too many args)
 *
 * @param builtin[in] The builtin to execute
 * @param cmd[in] The command node to execute
 *
 * @return The exit status code.
 */
uint8_t		execute_builtin(t_builtin builtin, t_cmd_node *cmd);

/**
 * Exits the current shell environment.
 *
 * Can take an optional argument of the exit status, if none is given, exits
 * with previous status.
 *
 * Returns 1 if too many arguments were given.
 * Exits with status 2 if the argument was too big or not numeric.
 *
 * @param cmd[in] The command node to execute
 *
 * @return Only returns 1 if too many arguments were given, otherwise exits.
 */
uint8_t		builtin_exit(t_cmd_node *cmd);
/**
 * Print a message on the standard output, with spaces between all arguments
 * and a newline on the end (if the flag `-n' was not given)
 *
 * @param cmd[in] The command node to execute
 *
 * @return Always returns 0
 */
uint8_t		builtin_echo(t_cmd_node *cmd);

/**
 * Print a message on stderr and exit with status
 */
uint8_t		builtin_err(char *cmd, char *arg, char *msg, uint8_t status);

#endif //BUILTINS_H
