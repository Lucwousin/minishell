/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 13:57:10 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/22 13:57:10 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>
#include <libft.h>
#include <stdbool.h>

/**
 * Split up the input string into something more manageable.
 * * operators (for us, that's pipes and io redirects)
 * * words (anything else)
 * * end of input (we're done)
 */
t_tokens	tokenize(const char *cmd)
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
	return (data);
}
