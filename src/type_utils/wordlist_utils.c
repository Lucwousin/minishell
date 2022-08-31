/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   wordlist_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/31 13:41:33 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/31 13:41:33 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <ms_types.h>
#include <libft.h>

t_wordlist	*new_wordlist(char *word, uint8_t flags)
{
	t_wordlist	*node;

	node = ft_calloc(1, sizeof(t_wordlist));
	if (node == NULL)
		return (NULL);
	node->flags = flags;
	node->word = word;
	return (node);
}

t_wordlist	*new_wordlist_token(const char *cmd, const t_token *token)
{
	char		*word;
	t_wordlist	*node;

	word = ft_substr(cmd, token->start, token->end - token->start + 1);
	if (word == NULL)
		return (NULL);
	node = new_wordlist(word, token->flags);
	if (node != NULL)
		return (node);
	free(word);
	return (NULL);
}

uint8_t	destroy_wordlist(t_wordlist *cur)
{
	t_wordlist	*next;

	cur = cur->next;
	while (cur != NULL)
	{
		next = cur->next;
		free(cur->word);
		free(cur);
		cur = next;
	}
	return (ERROR);
}

static char	*do_combine(t_wordlist *cur, t_wordlist *end, size_t idx)
{
	size_t	len;
	char	*ret;

	len = ft_strlen(cur->word);
	if (cur == end)
		ret = ft_calloc(idx + len + 1, sizeof(char));
	else
		ret = do_combine(cur->next, end, idx + len);
	if (ret == NULL)
		return (NULL);
	ft_memcpy(ret + idx, cur->word, len);
	return (ret);
}

bool	combine(t_wordlist *start, t_wordlist *end)
{
	t_wordlist	destroy;
	char		*combined;

	combined = do_combine(start, end, 0);
	if (combined == NULL)
		return (false);
	free(start->word);
	start->word = combined;
	destroy.next = start->next;
	start->next = end->next;
	if (start != end)
	{
		end->next = NULL;
		destroy_wordlist(&destroy);
	}
	return (true);
}
