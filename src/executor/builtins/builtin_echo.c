/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_echo.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/05 19:32:37 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/05 19:32:37 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <execute.h>
#include <libft.h>
#include <unistd.h>

#define FLAG_N	"-n"

static bool	should_print_newline(char **argv, size_t len, size_t *i)
{
	if (len == 2)
		return (true);
	if (ft_strncmp(argv[*i], FLAG_N, 3) != 0)
		return (true);
	*i = *i + 1;
	return (false);
}

uint8_t	builtin_echo(t_cmd_node *cmd)
{
	size_t	i;
	char	**argv;
	bool	print_newline;

	i = 1;
	argv = cmd->argv.arr;
	print_newline = should_print_newline(argv, cmd->argv.length, &i);
	while (argv[i])
	{
		ft_putstr_fd(argv[i++], STDOUT_FILENO);
		if (argv[i] != NULL)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (print_newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
