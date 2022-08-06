/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 18:29:45 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/30 18:29:45 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stdint.h>

# define PROMPT "minishell> "

typedef struct s_g {
	uint8_t	exit;
}	t_global;

extern t_global	g_globals;

void	minishell(int argc, char **argv);

#endif