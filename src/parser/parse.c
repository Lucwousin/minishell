/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/05 20:11:04 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/05 20:11:04 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <command.h>
#include <stdlib.h>
#include <unistd.h>

static const uint8_t	g_tokens_used[] = {
[END_OF_INPUT] = 1,
[WORD] = 1,
[OPERATOR] = 1,
[PIPE] = 2,
[RED_IN] = 2,
[RED_HD] = 1,
[RED_OUT] = 2,
[RED_APP] = 2
};

static void	set_redir(t_command *command, t_tokentype type, t_exp_token *arg)
{
	(void) command;
	(void) type;
	(void) arg;
	// TODO: create
	// We should also check for syntax errors here - these "token types" are
	// the only ones which NEED something else behind them
}

static void	add_cmd(t_dynarr *commands, t_command *command, t_dynarr *argv_arr)
{
	if (!dynarr_addone(argv_arr, NULL) || \
		!dynarr_finalize(argv_arr))
		exit(EXIT_FAILURE); // TODO: error
	command->argv = argv_arr->arr;
	dynarr_addone(commands, &command);
}

static size_t	parse_cmd(t_exp_token *expanded, t_dynarr *commands, size_t i)
{
	t_dynarr	argv_arr;
	t_command	command;

	command = (t_command){NULL, {STDIN_FILENO, STDOUT_FILENO}};
	if (!dynarr_create(&argv_arr, ARGV_INIT_SIZE, sizeof(char *)))
		exit(EXIT_FAILURE); // TODO: erorr
	while (expanded[i].type != END_OF_INPUT)
	{
		if (expanded[i].type == OPERATOR)
			exit(EXIT_FAILURE); // This is also a syntax error - not implemented operator
		if (expanded[i].type >= PIPE && expanded[i].type <= RED_APP)
			set_redir(&command, expanded[i].type, expanded + i + 1);
		if (expanded[i].type == WORD)
			dynarr_addone(&argv_arr, expanded[i].str);
		if (expanded[i].type == PIPE)
			break ;
		i += g_tokens_used[expanded[i].type];
	}
	if (argv_arr.length == 0)
		dynarr_delete(&argv_arr);
	else
		add_cmd(commands, &command, &argv_arr);
	return (i);
}

void	parse(t_exp_token *expanded, t_dynarr *commands)
{
	size_t		i;

	if (!dynarr_create(commands, COMMAND_INIT_SIZE, sizeof(t_command)))
		exit(EXIT_FAILURE); // TODO: erorr
	i = 0;
	while (expanded[i].type != END_OF_INPUT)
		i = parse_cmd(expanded, commands, i);
	if (!dynarr_finalize(commands))
		exit(EXIT_FAILURE); // todo: error
}
