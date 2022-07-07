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

#include <dynarr.h>
#include <minishell.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <token.h>
#include <command.h>

static void	print_command(void *command, void *arg)
{
	t_command *cmd = command;
	char **argv = cmd->argv;
	(void) arg;
	printf("---- Command ----\n");

	printf("%d fd in\n", cmd->fds[0]);
	printf("%d fd out\n", cmd->fds[1]);

	while (*argv != NULL)
		printf("%s\n", *argv++);

	printf("---- End cmd ----\n");
	free(cmd->argv);
}

static void	do_something_with_input(char *input)
{
	t_dynarr	tokens;
	t_exp_token	*expanded;
	t_dynarr	commands;
	
	tokenize(&tokens, input);
	expanded = expand(&tokens, input);
	dynarr_delete(&tokens);
	parse(expanded, &commands);
	for (int i = 0; expanded[i].type != END_OF_INPUT; i++)
		printf("%s\n", expanded[i].str);
	dynarr_foreach(&commands, print_command, NULL);
	dynarr_delete(&commands);
	for (int i = 0; expanded[i].type != END_OF_INPUT; i++)
		free(expanded[i].str);
	free(expanded);
	system("leaks minishell");
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

static void print_env_var(void *v, void *ign)
{
	(void) ign;
	
	t_env_var	*var = v;
	char *str = (char *) env_var_to_str(var);
	printf("%s\n", str);
	free(str);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_msh	msh;

	if (!init_environment(&msh))
		exit(EXIT_FAILURE);
	dynarr_foreach(&msh.envp, print_env_var, NULL);

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
		if (*input)
		{
			add_history(input);
			do_something_with_input(input);
			if (strcmp(input, "exit") == 0)
				break ;
		}
		free(input);
	}
	delete_environment(&msh.envp);
}