/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   remove_quotes.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/31 13:48:42 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/31 13:48:42 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <input.h>
#include <libft.h>
#include <stdio.h>

static bool	set_from_buf(t_wordlist *dst, t_wordlist *next, t_dynarr *buf)
{
	char	*word;

	if (!dynarr_addone(buf, ""))
		return (false);
	word = ft_strdup(buf->arr);
	buf->length = 0;
	if (word == NULL)
		return (false);
	free(dst->word);
	dst->word = word;
	dst->next = next;
	return (true);
}

static bool	add_words(t_wordlist *dst, t_wordlist *cur, t_dynarr *buf)
{
	const bool	quote = flag(cur->flags, IS_QUOTED);
	const bool	append = flag(cur->flags, APPEND_NEXT);
	t_wordlist	*next;
	size_t		len;

	if (flag(cur->flags, HAS_GLOB))
		return (dst == cur || set_from_buf(dst, cur, buf));
	len = ft_strlen(cur->word + quote);
	if (len != 0 && !dynarr_add(buf, cur->word + quote, len - quote))
		return (false);
	next = cur->next;
	if (dst != cur)
	{
		free(cur->word);
		free(cur);
	}
	if (append)
		return (add_words(dst, next, buf));
	return (set_from_buf(dst, next, buf));
}

bool	remove_quotes(t_wordlist *cur, t_dynarr *buf)
{
	while (cur->next)
	{
		cur = cur->next;
		if (!flag(cur->flags, APPEND_NEXT | IS_QUOTED))
			continue ;
		if (!add_words(cur, cur, buf))
		{
			perror("remove_quotes");
			return (false);
		}
	}
	return (true);
}
