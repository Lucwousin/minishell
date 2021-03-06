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

# include <stdbool.h>
# include <stdint.h>

# define MINISHELL_H
# define PROMPT "minishell> "

void	minishell(int argc, char **argv, char **envp);

#endif