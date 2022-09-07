/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/04 19:31:47 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/04 19:31:47 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <builtins.h>
#include <minishell.h>
#include <libft.h>
#include <error.h>

static const t_builtin_info	g_bi_info[] = {
[CD] = {"cd", 3, builtin_cd},
[ECHO] = {"echo", 5, builtin_echo},
[ENV] = {"env", 4, builtin_env},
[EXIT] = {"exit", 5, builtin_exit},
[EXPORT] = {"export", 7, builtin_export},
[PWD] = {"pwd", 4, builtin_pwd},
[UNSET] = {"unset", 6, builtin_unset},
};

uint8_t	builtin_err(
		const char *cmd, const char *arg, const char *msg, uint8_t status)
{
	const char	*err[4];
	size_t		i;

	i = 0;
	if (cmd)
		err[i++] = cmd;
	if (arg)
		err[i++] = arg;
	if (msg)
		err[i++] = msg;
	err[i] = NULL;
	print_error(err, false, true);
	return (status);
}

t_builtin	identify_command(char **argv)
{
	t_builtin	cur;

	if (*argv == NULL)
		return (NONE);
	cur = EXIT;
	while (cur != NONE)
	{
		if (ft_strncmp(*argv, g_bi_info[cur].name, g_bi_info[cur].len) == 0)
			return (cur);
		--cur;
	}
	return (NONE);
}

uint8_t	execute_builtin(t_builtin builtin, char **argv)
{
	g_env.exit = g_bi_info[builtin].fun(argv);
	if (g_env.exit != EXIT_SUCCESS
		&& (builtin == EXIT || builtin == EXPORT || builtin == UNSET))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
