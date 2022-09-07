/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/15 17:58:07 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/15 17:58:07 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <input.h>
#include <libft.h>
#include <unistd.h>
#include <stdio.h>

#define ERR_PRE				"minishell: "
#define SYNTAX_PRE			"syntax error near unexpected token `"
#define SYNTAX_SUF			"'\n"

static const char	*g_type_strs[] = {
[END_OF_INPUT] = "newline",
[PAR_OPEN] = "(",
[PAR_CLOSE] = ")",
[OR] = "||",
[AND] = "&&",
[PIPE] = "|",
[RED_IN] = "<",
[RED_HD] = "<<",
[RED_HD_Q] = "<<",
[RED_OUT] = ">",
[RED_APP] = ">>",
[WORD] = "(a word)"
};

bool	print_error(const char **args, bool use_perror, bool add_colons)
{
	size_t	i;

	ft_putstr_fd(ERR_PRE, STDERR_FILENO);
	i = 0;
	while (args[i])
	{
		if (args[i + 1] == NULL && use_perror)
			perror(args[i]);
		else
		{
			ft_putstr_fd(args[i], STDERR_FILENO);
			if (args[i + 1] && add_colons)
				ft_putstr_fd(": ", STDERR_FILENO);
		}
		++i;
	}
	if (!use_perror)
		ft_putchar_fd('\n', STDERR_FILENO);
	return (false);
}

bool	general_error(const char *where)
{
	const char	*err[] = {
		where,
		NULL
	};

	return (print_error(err, true, true));
}

void	syntax_error(t_tokentype type)
{
	const char	*err[] = {
		SYNTAX_PRE,
		g_type_strs[type],
		SYNTAX_SUF,
		NULL
	};

	print_error(err, false, false);
}
