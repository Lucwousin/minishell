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
# include <stdint.h>
# include <signal.h>

# define PROMPT "minishell> "

/*
 * Necessary info for the current shell environment
 *
 * All of this, except `interrupted' could theoretically be on the stack and not
 * in a global variable. I don't think feeding a struct into every function is a
 * better practice than using a global variable for this though, and I'd rather
 * not make my code extra messy for no valid reasons
 *
 * `interrupted'  Gets set to `true' if a SIGINT was received anywhere during
 *                input handling (that includes during execution)
 *
 * `exit'         Contains the last ran commands' exit status
 *
 * `vars'         Contains environment variables (environ.h)
 *
 * `orig_signals' Contains a copy of the original signal handlers (signals.h)
 *                This is necessary, as bash resets the signal handlers to the
 *                inherited values from the shell's parent
 */
typedef struct s_shell_environment {
	bool				interrupted;
	uint8_t				exit;
	t_dynarr			vars;
	struct sigaction	orig_signals[2];
}	t_sh_env;

extern t_sh_env	g_env;

/**
 * Main entry point of the process. This is the only function called by main.
 *
 * main is in a different file to make testing easier.
 */
uint8_t	minishell(int argc, char **argv);

/**
 * Executes the entire lex/expand/parse/execute stack.
 */
uint8_t	handle_input(const char *input);

#endif