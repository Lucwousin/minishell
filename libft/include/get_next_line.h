/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/20 12:59:16 by lsinke        #+#    #+#                 */
/*   Updated: 2022/04/20 12:59:16 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <libft.h>
# include <stdlib.h>

typedef struct s_content
{
	int		fd;
	char	*leftover;
}			t_content;

char	*get_next_line(int fd);

#endif