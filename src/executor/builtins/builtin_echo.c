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

uint8_t	builtin_echo(t_cmd_node *cmd)
{
	size_t	i;
	char	**argv;
	bool	print_newline;

	i = 1;
	argv = cmd->argv.arr;
	print_newline = cmd->argv.length == 2 || ft_strncmp(argv[i], FLAG_N, 3);
	if (!print_newline)
		++i;
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
