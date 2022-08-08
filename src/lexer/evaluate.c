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
		shallow_copy[i] = ((t_token *) dynarr_get(tokens, i))->type;
		++i;
	}
	return (shallow_copy);
}

static void	add_quoted_bit(t_tokentype *shallow)
{
	bool	in_q[2];
	size_t	quote_idx;
	size_t	idx;

	idx = 0;
	in_q[0] = false;
	in_q[1] = false;
	while (shallow[idx] != END_OF_INPUT)
	{
		if (toggle_quote(in_q, shallow[idx]) && in_q[shallow[idx] != SQUOTE])
			quote_idx = idx;
		else if (shallow[idx] != SQUOTE && in_q[0])
			shallow[idx] |= TOKEN_S_QUOTED;
		else if (shallow[idx] != DQUOTE && in_q[1])
			shallow[idx] |= TOKEN_D_QUOTED;
		if (shallow[++idx] != END_OF_INPUT || (!in_q[0] && !in_q[1]))
			continue ;
		toggle_quote(in_q, shallow[quote_idx]);
		while (--idx != quote_idx)
			shallow[idx] &= ~(TOKEN_S_QUOTED | TOKEN_D_QUOTED);
		idx++;
	}
}

static bool	is_not_invalid_quote(size_t idx, const t_tokentype *quoted)
{
	if (quoted[idx] != SQUOTE && quoted[idx] != DQUOTE)
		return (true);
	if (idx > 0 && (quoted[idx - 1] & (TOKEN_S_QUOTED | TOKEN_D_QUOTED)))
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
			if (quoted[idx] & (TOKEN_S_QUOTED | TOKEN_D_QUOTED)
				&& cur->type != VARIABLE)
				cur->type = WORD;
			if (len != idx)
				dynarr_set(tokens, len, cur);
			len++;
		}
		idx++;
	}
	return (dynarr_grow(tokens, len));
}

uint8_t	evaluate(t_dynarr *tokens)
{
	t_tokentype	*shallow;
	bool		rv;

	shallow = shallow_representation(tokens);
	if (shallow == NULL)
		return (EXIT_FAILURE);
	add_quoted_bit(shallow);
	rv = remove_whitespace(tokens, shallow);
	if (rv == false)
		dynarr_delete(tokens);
	free(shallow);
	if (rv)
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
}
