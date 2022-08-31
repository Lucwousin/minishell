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

#include <libft.h>
#include <unistd.h>

static bool	should_print_newline(char **argv, size_t *i)
{
	bool	print;
	size_t	j;

	print = true;
	while (argv[*i])
	{
		j = 0;
		if (argv[*i][j++] != '-')
			return (print);
		while (argv[*i][j] == 'n')
			++j;
		if (argv[*i][j] != '\0')
			return (print);
		print = false;
		*i = *i + 1;
	}
	return (print);
}

uint8_t	builtin_echo(char **argv)
{
	size_t	i;
	bool	print_newline;

	i = 1;
	print_newline = should_print_newline(argv, &i);
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
