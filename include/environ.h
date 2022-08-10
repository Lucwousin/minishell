/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   environ.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/04 18:14:24 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/04 18:14:24 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRON_H
# define ENVIRON_H

# include <stddef.h>
# include <stdint.h>

/**
 * Initialize our environment variables. Copy from old environ and store in the
 * shell environment global.
 *
 * @return ERROR on error, SUCCESS on success
 */
uint8_t		init_environment(void);

/**
 * Uninitialize our environment variables. Free the array and contents.
 */
void		clean_environment(void);

/**
 * Get the value for variable `name'
 *
 * @param name[in] the name of the variable to get the value of
 * @return The value of the variable, or NULL if the variable was not set
 */
const char	*get_variable(const char *name);

/**
 * Get the value for variable var, getting the name from the first `len' chars
 *
 * @param var[in] A string starting with a variable name
 * @param len[in] The length of the variable name
 * @return The value of the variable, or NULL if the variable was not set
 */
const char	*get_variable_value(const char *var, size_t len);

/**
 * Set or add a new variable to the environment
 *
 * @param var[in] A string in the name=value format
 * @param name_end[in] A pointer to the first character in the string that's not
 *                     part of the name (so, the '=')
 * @return SUCCESS if the variable was updated successfully, ERROR on error
 */
uint8_t		set_variable(char *var, char *name_end);

/**
 * Get a pointer to the entry of a variable for name `var' from the environment
 *
 * @param var[in] A string starting with the name of the variable
 * @param len[in] The length of the name of the variable
 * @return A pointer to an entry in the environment array, or NULL
 */
const char	**find_variable(const char *var, size_t len);

/**
 * Get a pointer to the first character that's not part of the variable name
 *
 * @param str[in] A string starting with a variable name
 *
 * @return The pointer to the first character that's not a valid variable name
 */
char		*var_name_end(const char *str);

#endif //ENVIRON_H
