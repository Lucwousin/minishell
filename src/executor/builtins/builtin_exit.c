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

static const char	*skip_whitespace(const char *arg)
{
	while (*arg == ' ' || *arg == '\t' || *arg == '\n')
		++arg;
	return (arg);
}

static bool	parse_status(const char *arg, uint8_t *status)
{
	int8_t		sign;
	uint64_t	parse;

	arg = skip_whitespace(arg);
	sign = 1;
	if (*arg == '-')
		sign = -1;
	if (*arg == '-' || *arg == '+')
		++arg;
	parse = 0;
	while (ft_isdigit(*arg))
	{
		parse *= 10;
		parse += (*arg++ - '0');
		if ((sign == 1 && parse > INT64_MAX) || (sign == -1 && parse > (uint64_t) INT64_MIN))
			return (false);
	}
	arg = skip_whitespace(arg);
	if (*arg != '\0')
		return (false);
	*status = sign * parse;
	return (true);
}

uint8_t	builtin_exit(char **argv)
{
	uint8_t	status;
	char	*arg;

	if (argv[1] == NULL)
		exit(g_globals.exit);
	if (argv[2] != NULL)
		return (builtin_err(PRE_EXIT, NULL, ERR_ARGS, ERROR));
	arg = argv[1];
	if (parse_status(arg, &status))
		exit(status);
	exit(builtin_err(PRE_EXIT, arg, ERR_NUMS, SYNTAX));
}
