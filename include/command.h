/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   command.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/05 20:06:02 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/05 20:06:02 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include <token.h>
# include <stdint.h>

# define COMMAND_INIT_SIZE	8
# define ARGV_INIT_SIZE		8

typedef struct s_command {
	char		**argv;
	int32_t		fds[2];
}	t_command;

void	parse(t_exp_token *expanded, t_dynarr *commands);

#endif //COMMAND_H
