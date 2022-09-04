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

#define SPACE_TAB	" \t"
#define WHITES		"\n\v\r\f"

static const char	*skip_chars(const char *chars, const char *arg)
{
	while (*arg && ft_strchr(chars, *arg))
		++arg;
	return (arg);
}

static bool	check_overflow(uint64_t parse, int8_t sign, char c)
{
	static const uint64_t	pos_max = 0x7fffffffffffffff;
	static const uint64_t	neg_max = 0x8000000000000000;
	uint64_t				cutoff;
	int						max_val;

	if (sign == 1)
		cutoff = pos_max;
	else
		cutoff = neg_max;
	max_val = (int) (cutoff % 10u) + '0';
	cutoff /= 10;
	return (parse > cutoff || (parse == cutoff && c > max_val));
}

static bool	parse_status(const char *arg, uint8_t *status)
{
	int8_t		sign;
	uint64_t	parse;

	arg = skip_chars(SPACE_TAB WHITES, arg);
	sign = 1;
	if (*arg == '-')
		sign = -1;
	if (*arg == '-' || *arg == '+')
		++arg;
	parse = 0;
	while (ft_isdigit(*arg))
	{
		if (check_overflow(parse, sign, *arg))
			return (false);
		parse *= 10;
		parse += (*arg++ - '0');
	}
	*status = sign * parse;
	arg = skip_chars(SPACE_TAB, arg);
	return (*arg == '\0');
}

uint8_t	builtin_exit(char **argv)
{
	uint8_t	status;
	char	*arg;

	if (argv[1] == NULL)
		exit(g_globals.exit);
	arg = argv[1];
	if (!parse_status(arg, &status))
		exit(builtin_err(PRE_EXIT, arg, ERR_NUMS, SYNTAX));
	if (argv[2] != NULL)
		return (builtin_err(PRE_EXIT, NULL, ERR_ARGS, ERROR));
	exit(status);
}
