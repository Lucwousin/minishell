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
#include <stdlib.h>
#include <stdio.h>

/**
 * Split up the input string into something more manageable.
 * * operators (for us, that's pipes and io redirects)
 * * words (anything else)
 * * end of input (we're done)
 */
void	tokenize(t_dynarr *tokens, const char *cmd)
{
	t_token		token;
	size_t		str_idx;

	if (!dynarr_create(tokens, TOKENS_INIT_SIZE, sizeof(t_token)))
		exit(EXIT_FAILURE); // TODO: better error handling
	str_idx = 0;
	while (true)
	{
		token.token = END_OF_INPUT;
		match_token(cmd, &str_idx, &token);
		if (token.token == END_OF_INPUT)
			break ;
		if (token.token == WORD && !dynarr_finalize(&token.sub))
			exit(EXIT_FAILURE); // you know the drill
		if (!dynarr_addone(tokens, &token))
			exit(EXIT_FAILURE); // TODO: better error handling
	}
	if (!dynarr_finalize(tokens)) // Shrink the array as small as possible
		exit(EXIT_FAILURE); // how many exits can we reach?
}
