/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_filenames.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/31 13:50:04 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/31 13:50:04 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <ms_types.h>
#include <libft.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// Prototype so dirent.h does not need to be included everywhere
bool	open_cwd(DIR **dst);

static bool	glob_match(t_wordlist *cur, t_wordlist *end, char *file)
{
	const char		*word = cur->word;
	const size_t	len = ft_strlen(word);

	if (!flag(cur->flags, HAS_GLOB))
		return (ft_strncmp(file, word, len) == 0
			&& (cur == end || glob_match(cur->next, end, file + len)));
	if (word[1] == '\0')
		return (cur == end || glob_match(cur->next, end, file));
	while (true)
	{
		file = ft_strchr(file, word[1]);
		if (file == NULL)
			return (false);
		if (ft_strncmp(file++, word + 1, len - flag(cur->flags, APPEND_NEXT)))
			continue ;
		file += len - 1;
		if (cur == end || glob_match(cur->next, end, file))
			return (true);
	}
}

static bool	\
	match_glob(t_wordlist *cur, t_wordlist *end, char *file, t_wordlist **dst)
{
	if (flag(cur->flags, HAS_GLOB) && file[0] == '.')
		return (true);
	if (!glob_match(cur, end, file))
		return (true);
	file = ft_strdup(file);
	if (file == NULL)
		return (false);
	dst[0]->next = new_wordlist(file, 0);
	if (dst[0]->next == NULL)
	{
		free(file);
		return (false);
	}
	dst[0] = dst[0]->next;
	return (true);
}

static bool \
	replace_words(t_wordlist *m, t_wordlist *m_e, t_wordlist *s, t_wordlist *e)
{
	if (m == m_e)
		return (combine(s, e));
	m_e->next = e->next;
	e->next = NULL;
	if (s->next != NULL)
		destroy_wordlist(s);
	free(s->word);
	*s = *m->next;
	free(m->next);
	return (true);
}

static bool	expand_globs(t_wordlist *start, t_wordlist *end)
{
	struct dirent	*entry;
	t_wordlist		matches;
	t_wordlist		*matches_end;
	DIR				*cwd;

	if (!open_cwd(&cwd))
		return (false);
	matches = (t_wordlist){};
	matches_end = &matches;
	while (true)
	{
		errno = 0;
		entry = readdir(cwd);
		if (entry == NULL)
			break ;
		if (!match_glob(start, end, entry->d_name, &matches_end))
			break ;
	}
	closedir(cwd);
	if (errno == 0)
		return (replace_words(&matches, matches_end, start, end));
	perror("expand_globs");
	destroy_wordlist(&matches);
	return (false);
}

bool	expand_filenames(t_wordlist *cur)
{
	t_wordlist	*next;
	t_wordlist	*end;
	bool		has_glob;

	next = cur->next;
	while (next)
	{
		has_glob = false;
		cur = next;
		end = cur;
		while (end)
		{
			has_glob |= end->flags & HAS_GLOB;
			next = end->next;
			if (!(end->flags & APPEND_NEXT))
				break ;
			end = next;
		}
		if (has_glob && !expand_globs(cur, end))
			return (false);
	}
	return (true);
}
