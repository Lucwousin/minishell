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
#include <libft.h>

static t_tokentype	*shallow_representation(t_dynarr *tokens)
{
	t_tokentype	*shallow_copy;
	size_t		i;

	shallow_copy = ft_calloc(tokens->length, sizeof(t_tokentype));
	if (shallow_copy == NULL)
		return (NULL);
	i = 0;
	while (i < tokens->length)
	{
		shallow_copy[i] = ((t_token *) dynarr_get(tokens, i))->token;
		++i;
	}
	return (shallow_copy);
}

static void	add_quoted_bit(t_tokentype *shallow)
{
	bool	in_quotes[2];
	size_t	idx;

	idx = 0;
	in_quotes[0] = false;
	in_quotes[1] = false;
	while (shallow[idx] != END_OF_INPUT)
	{
		if (shallow[idx] != SQUOTE && shallow[idx] != DQUOTE && in_quotes[0])
			shallow[idx] |= TOKEN_S_QUOTED;
		if (shallow[idx] != SQUOTE && shallow[idx] != DQUOTE && in_quotes[1])
			shallow[idx] |= TOKEN_D_QUOTED;
		if (shallow[idx] == SQUOTE && in_quotes[1] == false)
			in_quotes[0] = !in_quotes[0];
		if (shallow[idx] == DQUOTE && in_quotes[0] == false)
			in_quotes[1] = !in_quotes[1];
		++idx;
	}
	if (in_quotes[0] || in_quotes[1])
	{
		while (shallow[--idx] & (TOKEN_S_QUOTED | TOKEN_D_QUOTED))
			shallow[idx] &= ~(TOKEN_S_QUOTED | TOKEN_D_QUOTED);
	}
}

static bool	is_not_invalid_quote(size_t idx, const t_tokentype *quoted)
{
	if (quoted[idx] != SQUOTE && quoted[idx] != DQUOTE)
		return (true);
	if (idx > 0 && quoted[idx - 1] & (TOKEN_S_QUOTED | TOKEN_D_QUOTED))
		return (true);
	if (quoted[idx + 1] & (TOKEN_S_QUOTED | TOKEN_D_QUOTED))
		return (true);
	return (false);
}

static bool	remove_whitespace(t_dynarr *tokens, const t_tokentype *quoted)
{
	size_t	idx;
	size_t	len;
	t_token	*cur;

	idx = 0;
	len = 0;
	while (idx < tokens->length)
	{
		cur = dynarr_get(tokens, idx);
		if (quoted[idx] != WHITESPACE && is_not_invalid_quote(idx, quoted))
		{
			if (len != idx)
				dynarr_set(tokens, len, cur);
			len++;
		}
		idx++;
	}
	return (dynarr_grow(tokens, len));
}

bool	evaluate(t_dynarr *tokens)
{
	t_tokentype	*shallow;

	shallow = shallow_representation(tokens);
	if (shallow == NULL)
		return (false);
	add_quoted_bit(shallow);
	if (!remove_whitespace(tokens, shallow))
	{
		free(shallow);
		return (false);
	}
	free(shallow);
	return (true);
}
