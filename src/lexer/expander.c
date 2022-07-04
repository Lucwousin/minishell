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

#define ARRAY	0
#define INDEX	1
#define STRING	2

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
	if (str_index <= token->end)
		if (!dynarr_add(buf, (char *) str + str_index, token->end - str_index + 1))
			exit(EXIT_FAILURE); // todo: error
	if (token->token == WORD && (!dynarr_add(buf, "", 1) || !dynarr_finalize(buf)))
		exit(EXIT_FAILURE);
	return (buf->arr);
}

static void	expand_token(void *data, void *params[3])
{
	t_token		token;
	t_exp_token	*array;
	const char	*string;
	size_t		index;
	t_dynarr	buffer;

	ft_memcpy(&token, data, sizeof(t_token));
	buffer.capacity = 0;
	index = *((size_t *) params[INDEX]);
	*((size_t *) params[INDEX]) = index + 1;
	array = params[ARRAY];
	string = params[STRING];
	array[index].type = token.token;
	if (token.token != WORD || token.sub.length == 0)
		array[index].str = ft_substr(string, token.start, token.end - token.start + 1);
	else
		array[index].str = expand_subs(&buffer, &token, string);
	if (array[index].str == NULL)
		exit(EXIT_FAILURE); //todo: error handling
}

t_exp_token	*expand(t_dynarr *tokens, const char *cmd)
{
	t_exp_token	*expanded;
	void		*params[3];
	size_t		index;

	index = 0;
	expanded = malloc((tokens->capacity + 1) * sizeof(t_exp_token));
	if (expanded == NULL)
		exit(EXIT_FAILURE); // todo: better error handling
	expanded[tokens->capacity].type = END_OF_INPUT;
	params[ARRAY] = expanded;
	params[INDEX] = &index;
	params[STRING] = (void *) cmd;
	dynarr_foreach(tokens, (void (*)(void *, void *)) expand_token, params);
	return (params[ARRAY]);
}