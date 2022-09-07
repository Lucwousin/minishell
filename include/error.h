/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/07 17:56:59 by lsinke        #+#    #+#                 */
/*   Updated: 2022/09/07 17:56:59 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H
# include <ms_types.h>

bool	print_error(const char **args, bool use_perror, bool add_colons);
bool	general_error(const char *where);
void	syntax_error(t_tokentype type);

#endif //ERROR_H
