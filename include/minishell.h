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

# include <dynarr.h>
# include <stdbool.h>
# include <stdint.h>

# define PROMPT "minishell> "

typedef struct s_shell_environment {
	uint8_t		exit;
	t_dynarr	vars;
}	t_sh_env;

extern t_sh_env	g_globals;

uint8_t	minishell(int argc, char **argv);

uint8_t	handle_input(const char *input);

#endif