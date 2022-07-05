/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/01 17:41:05 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/01 17:41:05 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>
#include <stdlib.h>
#include "libft.h"

static void	expand_variable(t_dynarr *buffer, t_token *token, const char *str)
{
	char	*var_name;
	char	*var_value;

	token->start++;
	var_name = ft_substr(str, token->start, token->end - token->start + 1);
	if (var_name == NULL)
		exit(EXIT_FAILURE); // TODO: error
	var_value = getenv(var_name); // TODO: when we modify environment, we need to get hte modified version
	free(var_name);
	if (var_value == NULL)
		return ;
	if (!dynarr_add(buffer, var_value, ft_strlen(var_value)))
		exit(EXIT_FAILURE);
}

static char	*expand_subs(t_dynarr *buf, t_token *token, const char *str)
{
	t_token		*sub;
	size_t		str_index;
	size_t		sub_index;

	if (buf->capacity == 0)
		dynarr_create(buf, token->end - token->start + 2, sizeof(char));
	sub_index = 0;
	str_index = token->start;
	while (sub_index < token->sub.length)
	{
		sub = dynarr_get(&token->sub, sub_index++);
		if (sub->start > str_index && \
			!dynarr_add(buf, (void *) str + str_index, sub->start - str_index))
			exit(EXIT_FAILURE); // todo: error
		str_index = sub->end + 1;
		if (sub->token == VARIABLE)
			expand_variable(buf, sub, str);
		else if (sub->start - sub->end > 1)
		{
			sub->start += 1;
			sub->end -= 1;
			expand_subs(buf, sub, str);
		}
	}
	if (str_index <= token->end && \
		!dynarr_add(buf, (void *) str + str_index, token->end - str_index + 1))
		exit(EXIT_FAILURE); // todo: error
	if (token->token == WORD && (!dynarr_addone(buf, "") || !dynarr_finalize(buf)))
		exit(EXIT_FAILURE);
	return (buf->arr);
}

static void	expand_token(void *data, void *arg_p)
{
	t_exp_arg	*arg;
	t_exp_token	*exp_token;
	t_token		token;
	t_dynarr	buffer;

	ft_memcpy(&token, data, sizeof(t_token));
	arg = arg_p;
	buffer.capacity = 0;
	exp_token = arg->expanded + arg->index;
	exp_token->type = token.token;
	if (token.token == WORD && token.sub.length != 0)
		exp_token->str = expand_subs(&buffer, &token, arg->cmd);
	else
		exp_token->str = ft_substr(
				arg->cmd, token.start, token.end - token.start + 1);
	if (exp_token->str == NULL)
		exit(EXIT_FAILURE); //todo: error handling
}

t_exp_token	*expand(t_dynarr *tokens, const char *cmd)
{
	t_exp_arg	arg;

	arg.cmd = cmd;
	arg.index = 0;
	arg.expanded = malloc((tokens->capacity + 1) * sizeof(t_exp_token));
	if (arg.expanded == NULL)
		exit(EXIT_FAILURE); // todo: better error handling
	arg.expanded[tokens->capacity].type = END_OF_INPUT;
	dynarr_foreach(tokens, expand_token, &arg);
	return (arg.expanded);
}
