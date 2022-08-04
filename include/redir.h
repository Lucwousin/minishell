/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/04 14:58:10 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/04 14:58:10 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIR_H
# define REDIR_H
# include <stdbool.h>

bool	create_heredoc(char **dst, bool expand);
bool	read_heredoc(char *file, char *delim, bool expand);

#endif //REDIR_H
