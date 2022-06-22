/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: user <user@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 13:57:10 by user          #+#    #+#                 */
/*   Updated: 2022/06/22 13:57:10 by user          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "libft.h"
#include <stdbool.h>

static void	resize_tokens(t_tokens *data, size_t new_len)
{
	t_codepoint	*new_allocated;

	new_allocated = ft_realloc(
			data->tokens,
			data->tokens_size * sizeof(t_codepoint),
			new_len * sizeof(t_codepoint));
	if (new_allocated == NULL)
		// TODO: This should probably display an error msg
		// Note that realloc does not free the pointer if an error occured
		exit(EXIT_FAILURE);
	data->tokens = new_allocated;
	data->tokens_size = new_len;
}

/**
 * Split up the input string into something more manageable.
 * * operators (for us, that's pipes and io redirects)
 * * words (anything else)
 * * end of input (we're done)
 */
t_codepoint	*tokenize(const char *cmd)
{
	t_tokens	data;
	t_codepoint	token;
	size_t		token_idx;
	size_t		str_idx;

	data.tokens = NULL;
	data.tokens_size = 0;
	resize_tokens(&data, TOKENS_INIT_SIZE);
	token_idx = 0;
	str_idx = 0;
	while (true)
	{
		token.token = END_OF_INPUT;
		match_token(cmd, &str_idx, &token);
		if (token.token == END_OF_INPUT)
			break ;
		if (token_idx == data.tokens_size)
			resize_tokens(&data, data.tokens_size * 2);
		data.tokens[token_idx++] = token;
	}
	return (data.tokens);
}
