/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/06 21:10:33 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/06 21:10:33 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <dynarr.h>

# define ENV_INIT_SIZE	64

typedef struct s_minishell	t_msh;

/**
 * A environment variable.
 * 
 * _key_ is this variable's name (PATH for instance)
 * _values_ are this variable's (string) values
 */
typedef struct s_env_var {
	char	*key;
	char	*val;
}	t_env_var;

/**
 * Initialize the environment variables. Reads from the environ global and
 * parses all the variables.
 * 
 * @param msh The shell data
 * @return true if everything went as planned, false if an error occurred
 */
bool		init_environment(t_msh *msh);

/**
 * Uninitialize the environment variables. Frees all key/value pairs and the
 * array containing them.
 *
 * @param msh The shell data
 */
void		delete_environment(t_dynarr *envp);

/**
 * Parse an environment variable from a string like in _environ_
 * 
 * @param envs The environment variable array
 * @param str The string representation of this environment variable
 * @return true if everything went as planned, false if an error occurred
 */
bool		parse_env_var(t_dynarr *envs, const char *str);

/**
 * Do the opposite of parse_env_var: convert to a string like HOME=/Users/User
 * 
 * @param var The environment variable we want the string representation of
 * @return The string representation of this variable, or NULL if error
 */
char		*env_var_to_str(t_env_var *var);

/**
 * Add a new environment variable, or change the value if it is already set.
 * 
 * @param envs The environment variable array
 * @param key The name of the variable we want to set
 * @param value The value we want to set this variable to
 * @return true if everything went as planned, false if an error occurred
 */
bool		set_env_var(t_dynarr *envs, const char *key, char *value);

/**
 * Get an environment variable from the environment variable array
 * 
 * @param envs The environment variable array
 * @param key The name of the variable we want to get
 * @return a pointer to the variable, or NULL if it was not set
 */
t_env_var	*get_env_var(t_dynarr *envs, const char *key);

/**
 * Get the value of an environment variable
 * 
 * @param msh The shell data; necessary for $?
 * @param key The name of the variable we want to get
 * @return a pointer to the variable, or NULL if it was not set or on error
 */
const char	*get_env_var_val(t_msh *msh, const char *key);

const char	**get_env_var_vals(t_dynarr *envs, const char *key);

#endif //ENV_H
