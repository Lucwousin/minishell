/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_exit.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/05 19:32:37 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/05 19:32:37 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>
#include <minishell.h>
#include <libft.h>
#include <stdlib.h>

#define PRE_EXIT	"exit: "
#define ERR_ARGS	"too many arguments"
#define ERR_NUMS	": numeric argument required"

static bool	parse_status(const char *arg, uint8_t *status)
{
	int8_t	sign;
	int64_t	parse;

	sign = 1;
	if (*arg == '-')
		sign = -1;
	if (*arg == '-' || *arg == '+')
		++arg;
	parse = 0;
	while (*arg)
	{
		if (!ft_isdigit(*arg))
			return (false);
		parse *= 10;
		parse += (*arg++ - '0');
		if (parse < 0)
			return (false);
	}
	*status = sign * parse;
	return (true);
}

uint8_t	builtin_exit(t_cmd_node *cmd)
{
	uint8_t	status;
	char	*arg;

	if (cmd->argv.length > 3)
		return (builtin_err(PRE_EXIT, NULL, ERR_ARGS, ERROR));
	if (cmd->argv.length == 2)
		exit(g_globals.exit);
	arg = ((char **) cmd->argv.arr)[1];
	if (parse_status(arg, &status))
		exit(status);
	exit(builtin_err(PRE_EXIT, arg, ERR_NUMS, SYNTAX));
}
