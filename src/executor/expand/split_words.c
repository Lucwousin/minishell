/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   split_words.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/31 13:48:36 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/31 13:48:36 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <input.h>
#include <libft.h>
#include <stdio.h>

#define IFS	" \t\n"

//TODO: export A=' ' && echo "@"$A "@"
// ^ prints @  @ instead of @ @
// echo "@"$A"@"  -- dit ook, empty var niet correct removed!

static bool	add_word(t_wordlist *next, t_wordlist **prev, char *str, size_t len)
{
	t_wordlist	*node;
	char		*word;
	t_flag		flags;

	flags = next->flags;
	if (ft_strchr(IFS, str[len]))
		flags &= ~APPEND_NEXT;
	next->flags &= ~HAS_GLOB;
	word = ft_substr(str, 0, len);
	node = new_wordlist(word, flags);
	if (node == NULL || word == NULL)
	{
		perror("split_words");
		free(node);
		free(word);
		return (false);
	}
	prev[0]->next = node;
	node->next = next;
	prev[0] = node;
	return (true);
}

static bool	split_word(t_wordlist *cur, t_wordlist *prev)
{
	char		*str;
	size_t		length;

	str = cur->word;
	while (*str)
	{
		length = 0;
		while (*str && ft_strchr(IFS, *str))
			++str;
		while (str[length] && ft_strchr(IFS, str[length]) == NULL)
			++length;
		if (str[length] == '\0' && str == cur->word)
			return (true);
		if (!add_word(cur, &prev, str, length))
			return (false);
		str += length;
	}
	prev->next = cur->next;
	free(cur->word);
	free(cur);
	return (true);
}

bool	split_words(t_wordlist *cur)
{
	t_wordlist	*prev;

	while (cur && cur->next)
	{
		prev = cur;
		cur = cur->next;
		if (!(cur->flags & HAS_VAR) || (cur->flags & D_QUOTED))
			continue ;
		if (!split_word(cur, prev))
			return (false);
		cur = prev->next;
	}
	return (true);
}
