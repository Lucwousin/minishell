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
#include <unistd.h>
#include <libft.h>

static const char			*g_builtin_names[] = {
[CD] = "CD",
[ECHO] = "ECHO",
[ENV] = "ENV",
[EXIT] = "EXIT",
[EXPORT] = "EXPORT",
[PWD] = "PWD",
[UNSET] = "UNSET",
};

static const t_builtinfun	g_builtin_lut[] = {
[CD] = builtin_cd,
[ECHO] = builtin_echo,
[ENV] = builtin_echo,
[EXIT] = builtin_exit,
[EXPORT] = builtin_echo,
[PWD] = builtin_echo,
[UNSET] = builtin_echo,
};

uint8_t	builtin_err(char *cmd, char *arg, char *msg, uint8_t status)
{
	if (cmd)
		ft_putstr_fd(cmd, STDERR_FILENO);
	if (arg)
		ft_putstr_fd(arg, STDERR_FILENO);
	if (msg)
		ft_putstr_fd(msg, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	return (status);
}

static bool	equals_ignore_case(const char *a, const char *b)
{
	while (ft_toupper(*a++) == *b++)
		if (*a == '\0')
			return (true);
	return (false);
}

t_builtin	identify_command(char **argv)
{
	t_builtin	cur;

	if (*argv == NULL)
		return (NONE);
	cur = EXIT;
	while (cur != NONE)
	{
		if (equals_ignore_case(*argv, g_builtin_names[cur]))
			return (cur);
		--cur;
	}
	return (NONE);
}

uint8_t	execute_builtin(t_builtin builtin, t_cmd_node *cmd)
{
	g_globals.exit = g_builtin_lut[builtin](cmd);
	if (g_globals.exit != EXIT_SUCCESS
		&& (builtin == EXIT || builtin == EXPORT))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
