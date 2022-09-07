/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_export.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/08 18:31:22 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/08 18:31:22 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <execute.h>
#include <builtins.h>
#include <environ.h>
#include <libft.h>
#include <stdio.h>

#define PRE_EXPORT	"export: "
#define ERR_INVAL	": not a valid identifier"
#define WARN_IMPL1	": export without value exports a shell variable, "
#define WARN_IMPL2	"which are not implemented. ignoring"

static void	sort_strings(char **strs, size_t len)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (i < len)
	{
		j = i + 1;
		while (j < len)
		{
			if (ft_strncmp(strs[i], strs[j], SIZE_MAX) < 0)
			{
				tmp = strs[i];
				strs[i] = strs[j];
				strs[j] = tmp;
			}
			++j;
		}
		++i;
	}
}

/*
 * Export in bash correctly escapes special characters!
 *
 * I don't. We don't need to handle them as input so why handle them as output?
 *
 * Export without arguments prints the exported variables with declare -x %s=%s!
 *
 * I don't. We did not implement that builtin so that output makes no sense.
 */
static void	print_var(char *var)
{
	char	*equals;
	int32_t	name_len;

	equals = ft_strchr(var, '=');
	name_len = (int32_t)(equals - var + 1);
	printf("export %.*s\"%s\"\n", name_len, var, equals + 1);
}

static uint8_t	export_print(char **environ, size_t len)
{
	char	**sorted;

	sorted = malloc((len + 1) * sizeof(char *));
	if (sorted == NULL)
	{
		perror("export");
		return (ERROR);
	}
	ft_memcpy(sorted, environ, (len + 1) * sizeof(char *));
	sort_strings(sorted, len);
	while (len--)
		print_var(sorted[len]);
	free(sorted);
	return (SUCCESS);
}

static uint8_t	try_set_env(char *var)
{
	char	*name_end;

	name_end = var_name_end(var);
	if (var == name_end || (*name_end != '\0' && *name_end != '='))
		return (builtin_err(PRE_EXPORT, var, ERR_INVAL, ERROR));
	if (*name_end != '=')
		return (builtin_err(PRE_EXPORT, var, WARN_IMPL1 WARN_IMPL2, SUCCESS));
	return (set_variable(var, name_end));
}

uint8_t	builtin_export(char **argv)
{
	uint8_t		status;

	if (argv[1] == NULL)
		return (export_print(g_env.vars.arr, g_env.vars.length - 1));
	status = SUCCESS;
	while (*(++argv))
		status |= try_set_env(*argv);
	return (status);
}
