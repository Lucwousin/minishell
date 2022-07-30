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

#include <parse.h>

#ifndef EXECUTE_H
# define EXECUTE_H

void	execute(t_ast_node *root_node, int32_t *exit);

void	execute_command(t_ast_node *node);

#endif
