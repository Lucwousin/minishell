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

# include <ms_types.h>

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

typedef uint8_t	(*t_builtinfun)(char **);

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
 * @return EXIT_FAILURE on error, EXIT_SUCCESS otherwise
 */
uint8_t		execute_builtin(t_builtin builtin, char **argv);

/**
 * Change the current working directory. Only works with a relative or absolute
 * path.
 * 
 * @param cmd[in] The command node to execute
 * 
 * @return The exit status
 */
uint8_t		builtin_cd(char **argv);
/**
 * (This is a "special" builtin)
 *
 * Print a message on the standard output, with spaces between all arguments
 * and a newline on the end (if the flag `-n' was not given)
 *
 * @param cmd[in] The command node to execute
 *
 * @return Always returns 0
 */
uint8_t		builtin_echo(char **argv);
/**
 * Print all environment variables.
 *
 * This is not normally a builtin, so I don't really get why we have to make it
 * especially as it's pretty much the same as no-args export (semi-undefined).
 *
 * All options/arguments are ignored.
 *
 * @param cmd[in] The command node to execute
 *
 * @return Always returns 0
 */
uint8_t		builtin_env(char **argv);
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
uint8_t		builtin_exit(char **argv);
/**
 * (This is a "special" builtin)
 *
 * Export a shell variable to the environment variables of child processes
 *
 * This is kind of a weird requirement for this project. We don't need to
 * implement shell variables - just environment variables - but it's possible
 * to mark not-yet-assigned variables as exported in bash. The only way to
 * assign variables using _this_ shell is by using the export command and
 * actually entering key=value pairs. That's why I decided to not implement
 * this - it just doesn't make sense.
 *
 * @param cmd[in] The command node to execute
 *
 * @return Returns SUCCESS if everything went alright, ERROR if an error
 * occurred, or a invalid variable name is given.
 */
uint8_t		builtin_export(char **argv);
/**
 * Get the current working directory
 *
 * @param cmd[in] The command node to execute
 *
 * @return Returns SUCCESS if everything went alright, ERROR if an error
 * occurred
 */
uint8_t		builtin_pwd(char **argv);
/**
 * (This is a "special" builtin)
 *
 * Unset a shell/environment variable. As shell variables are not implemented
 * this only applies to environment variables.
 *
 * @param cmd[in] The command node to execute
 *
 * @return SUCCESS on success, ERROR if at least one name could not be unset.
 *         Because we do not have readonly variables, always returns SUCCESS
 */
uint8_t		builtin_unset(char **argv);

/**
 * Print a message on stderr and exit with status
 */
uint8_t		builtin_err(char *cmd, char *arg, char *msg, uint8_t status);

#endif //BUILTINS_H
