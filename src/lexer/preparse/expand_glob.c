/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_glob.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/22 14:41:23 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/22 14:41:23 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>
#include <libft.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

static bool	add_match(t_preparser *pp, const char file[])
{
	t_exp_tok	newtok;

	newtok = (t_exp_tok){WORD, ft_strdup(file)};
	if (newtok.str == NULL)
		return (false);
	return (dynarr_addone(pp->output, &newtok));
}

static bool	open_cwd(DIR **dst)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (false);
	*dst = opendir(cwd);
	free(cwd);
	return (*dst != NULL);
}

static bool	glob_match_next(t_preparser *pp, char *file_str, uint8_t glob_idx)
{
	char	*glob_str;
	size_t	match_len;

	glob_str = pp->cur.str + pp->globs[glob_idx] + 1;
	if (*glob_str == '\0')
		return (true);
	if (pp->globs[glob_idx] + 1 == pp->globs[glob_idx + 1])
		return (glob_match_next(pp, file_str, glob_idx + 1));
	if (glob_idx + 1 < pp->glob_count)
		match_len = pp->globs[glob_idx + 1] - (glob_str - pp->cur.str);
	else
		match_len = ft_strlen(glob_str) + 1;
	while (*file_str)
	{
		if (*file_str == *glob_str
			&& ft_strncmp(glob_str, file_str, match_len) == 0)
			if (glob_idx == pp->glob_count - 1 || \
				glob_match_next(pp, file_str + match_len, glob_idx + 1))
				return (true);
		++file_str;
	}
	return (false);
}

static bool	glob_match(t_preparser *pp, char *file_str)
{
	char	*glob_str;
	size_t	prefix_len;

	glob_str = pp->cur.str;
	prefix_len = pp->cur.str + pp->globs[0] - glob_str;
	if (ft_strncmp(glob_str, file_str, prefix_len) != 0)
		return (false);
	if (glob_str == pp->cur.str + pp->globs[0] && file_str[0] == '.')
		return (false);
	if (!glob_match_next(pp, file_str + prefix_len, 0))
		return (false);
	return (add_match(pp, file_str));
}

bool	expand_globs(t_preparser *pp)
{
	size_t			matches;
	DIR				*cwd_stream;
	struct dirent	*entry;

	if (pp->glob_count == 0)
		return (dynarr_addone(pp->output, &pp->cur));
	if (!open_cwd(&cwd_stream))
		return (false);
	matches = 0;
	while (true)
	{
		errno = 0;
		entry = readdir(cwd_stream);
		if (entry == NULL)
			break ;
		matches += glob_match(pp, entry->d_name);
	}
	closedir(cwd_stream);
	if (errno != 0)
		return (false);
	pp->glob_count = 0;
	if (matches == 0)
		return (dynarr_addone(pp->output, &pp->cur));
	return (free(pp->cur.str), true);
}
