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

/**
 * Split up the input string into something more manageable.
 * * operators (for us, that's pipes and io redirects)
 * * words (anything else)
 * * end of input (we're done)
 */
t_dynarr	tokenize(const char *cmd)
{
	t_dynarr	data;
	t_codepoint	token;
	size_t		str_idx;

	dynarr_create(&data, TOKENS_INIT_SIZE, sizeof(t_codepoint));
	str_idx = 0;
	while (true)
	{
		token.token = END_OF_INPUT;
		match_token(cmd, &str_idx, &token);
		if (token.token == END_OF_INPUT)
			break ;
		dynarr_add(&data, &token, 1);
	}
	return (data);
}
