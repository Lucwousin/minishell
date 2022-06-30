/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 18:29:19 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/30 18:29:19 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

static void	do_something_with_input(char *input)
{
	(void) input;
	// Lexer
	// Parser
	// Executor
	// This will probably be fed a struct used for ~~everything~~?
	// At least the env params, a pointer we can put the exit code
	// Probably dup'd file descriptors for stdin, stdout and stderr (do we need to handle 2> or &>?)
	// I still think the pid's of child processes should probably be the global, so we can kill them
	// if we receive Ctrl+C
	// We probably need bool in the struct for whether or not we need to exit as well (or we can just clean up in the function handling that, probably)

}

int	main(int argc, char **argv, char **envp)
{
	char	*input;

	(void) argc;
	(void) argv;
	(void) envp; // We are going to need this in the executor (execve needs it as param)
	// Init state struct, dup stdin stdout stderr?
	// Init signal handlers
	while (true)
	{
		input = readline(PROMPT);
		if (!input)
			break ;
		if	(*input)
			add_history(input);
		do_something_with_input(input);
		free(input);
	}
}