/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_variables.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/31 13:49:49 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/31 13:49:49 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <environ.h>
#include <libft.h>
#include <error.h>

static bool	put(t_dynarr *buf, const char *str, size_t len)
{
	if (len == SIZE_MAX)
		len = ft_strlen(str);
	if (dynarr_add(buf, str, len))
		return (true);
	return (general_error("expand_variables"));
}

static bool	add_var(const char **cmd_p, t_dynarr *buf, t_wordlist *cur)
{
	const char	*cmd = (*cmd_p) + 1;
	const char	*var_end = var_name_end(cmd);
	const char	*var_val;

	*cmd_p = var_end;
	if (var_end == cmd)
	{
		if ((*var_end != '?' && !ft_isdigit(*var_end))
			&& !(*var_end == '\0' && flag(cur->flags, APPEND_NEXT)))
			return (put(buf, "$", 1));
		(*cmd_p)++;
		if (*var_end == '?')
			return (put(buf, get_variable("?"), SIZE_MAX));
		return (true);
	}
	var_val = get_variable_value(cmd, var_end - cmd);
	if (var_val == NULL)
		return (true);
	return (put(buf, var_val, SIZE_MAX));
}

static bool	finish(t_wordlist **cur, t_wordlist *prev, t_dynarr *buf)
{
	free(cur[0]->word);
	cur[0]->word = NULL;
	if (buf->length == 0)
	{
		if (!(cur[0]->flags & APPEND_NEXT))
			prev->flags &= ~APPEND_NEXT;
		prev->next = cur[0]->next;
		free(*cur);
		*cur = NULL;
		return (true);
	}
	if (!put(buf, "", 1))
		return (false);
	buf->length = 0;
	cur[0]->word = ft_strdup(buf->arr);
	if (cur[0]->word == NULL)
		return (general_error("expand_variables"));
	if (ft_strchr(" \n\t", *cur[0]->word))
		prev->flags &= ~APPEND_NEXT;
	return (true);
}

static bool	expand_word(t_wordlist **cur_p, t_wordlist *prev, t_dynarr *buf)
{
	const char	*idx;
	const char	*var;
	t_wordlist	*cur;

	cur = *cur_p;
	if (!(cur->flags & HAS_VAR))
		return (true);
	idx = cur->word;
	while (true)
	{
		var = ft_strchr(idx, '$');
		if (var == NULL
			&& put(buf, idx, SIZE_MAX)
			&& finish(cur_p, prev, buf))
			return (true);
		else if (var == NULL
			|| !put(buf, idx, var - idx)
			|| !add_var(&var, buf, cur))
			return (false);
		idx = var;
	}
}

bool	expand_variables(t_wordlist *cur, t_dynarr *buf)
{
	t_wordlist	*prev;

	prev = cur;
	while (prev->next)
	{
		cur = prev->next;
		if (!expand_word(&cur, prev, buf))
			return (false);
		if (cur != NULL)
			prev = cur;
	}
	return (true);
}
