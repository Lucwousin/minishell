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
# define PROMPT "minishell> "

# include <env.h>
# include <stdbool.h>
# include <stdint.h>

typedef struct s_minishell {
	t_dynarr	envp;
}	t_msh;

#endif