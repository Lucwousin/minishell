/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   evaluate.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/12 17:43:33 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/12 17:43:33 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>

bool	evaluate(t_dynarr *tokens)
{
	bool	in_quote[2];
	t_token	*token;
	size_t	i;
	
	in_quote[0] = false;
	in_quote[1] = false;
	i = 0;
	while (true)
	{
		token = dynarr_get(tokens, i);
		if (token->token == SQUOTE && !in_quote[1])
			in_quote[0] = !in_quote[0];
		else if (token->token == DQUOTE && !in_quote[0])
			in_quote[1] = !in_quote[1];
		else
		{
			
		}
	}
}