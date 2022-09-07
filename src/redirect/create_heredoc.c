/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   create_heredoc.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/04 14:57:34 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/04 14:57:34 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <redir.h>
#include <signals.h>
#include <libft.h>
#include <unistd.h>
#include <error.h>

#define HEX_CHARS		"0123456789abcdef"
#define PREFIX			"/tmp/ms_"
#define PREFIX_L		8
#define NAME_TOO_LONG	"Heredoc delimiter is too long! Can't generate filename"

// Prototype so execute.h does not need to be included
bool	remove_quotes(t_wordlist *cur, t_dynarr *buf);

static void	add_pointer(char *name, uint64_t pointer)
{
	size_t	n;

	n = sizeof(size_t) * 2;
	while (n--)
	{
		name[n] = HEX_CHARS[pointer % 0x10];
		pointer /= 0x10;
	}
}

static bool	create_name(t_redir *redir, char *delim, size_t delim_len)
{
	size_t	name_len;
	char	*name;

	name_len = 9 + delim_len + sizeof(size_t) * 2;
	if (name_len >= 0xFF + 6)
	{
		free(delim);
		ft_putendl_fd(NAME_TOO_LONG, STDERR_FILENO);
		return (false);
	}
	name = ft_calloc(name_len, sizeof(char));
	if (name == NULL)
	{
		free(delim);
		return (general_error("heredoc create_name"));
	}
	ft_memcpy(name, PREFIX, PREFIX_L);
	ft_memcpy(name + PREFIX_L, delim, delim_len);
	add_pointer(name + PREFIX_L + delim_len, (uint64_t) redir);
	redir->hd.file = name;
	return (true);
}

static bool	create_delim(t_redir *redir)
{
	t_dynarr	buf;
	bool		success;

	success = dynarr_create(&buf, 16, sizeof(char));
	if (success)
		success = remove_quotes(&redir->wl, &buf);
	dynarr_delete(&buf);
	if (success)
		return (true);
	destroy_wordlist(&redir->wl);
	return (general_error("create_delim"));
}

bool	create_heredoc(t_redir *redir)
{
	char	*delim;
	size_t	delim_len;

	if (!create_delim(redir))
		return (false);
	delim = redir->wl.next->word;
	delim_len = ft_strlen(delim);
	free(redir->wl.next);
	redir->wl.next = NULL;
	if (!create_name(redir, delim, delim_len))
		return (false);
	if (!read_heredoc(redir, delim, delim_len))
		return (false);
	free(delim);
	return (true);
}
