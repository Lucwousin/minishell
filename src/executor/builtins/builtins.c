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
#include <unistd.h>
#include <libft.h>

#define ERR_PRE	"minishell: "

static const char			*g_builtin_names[] = {
[CD] = "cd",
[ECHO] = "echo",
[ENV] = "env",
[EXIT] = "exit",
[EXPORT] = "export",
[PWD] = "pwd",
[UNSET] = "unset",
};

static const uint8_t		g_builtin_lengths[] = {
[CD] = 3,
[ECHO] = 5,
[ENV] = 4,
[EXIT] = 5,
[EXPORT] = 7,
[PWD] = 4,
[UNSET] = 6,
};

static const t_builtinfun	g_builtin_lut[] = {
[CD] = builtin_cd,
[ECHO] = builtin_echo,
[ENV] = builtin_env,
[EXIT] = builtin_exit,
[EXPORT] = builtin_export,
[PWD] = builtin_pwd,
[UNSET] = builtin_unset,
};

uint8_t	builtin_err(char *cmd, char *arg, char *msg, uint8_t status)
{
	ft_putstr_fd(ERR_PRE, STDERR_FILENO);
	if (cmd)
		ft_putstr_fd(cmd, STDERR_FILENO);
	if (arg)
		ft_putstr_fd(arg, STDERR_FILENO);
	if (msg)
		ft_putstr_fd(msg, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
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
		if (ft_strncmp(*argv, g_builtin_names[cur], g_builtin_lengths[cur]) == 0)
			return (cur);
		--cur;
	}
	return (NONE);
}

uint8_t	execute_builtin(t_builtin builtin, char **argv)
{
	g_globals.exit = g_builtin_lut[builtin](argv);
	if (g_globals.exit != EXIT_SUCCESS
		&& (builtin == EXIT || builtin == EXPORT || builtin == UNSET))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
