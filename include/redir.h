/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/04 14:58:10 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/04 14:58:10 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIR_H
# define REDIR_H

# include <ms_types.h>
# include <stdnoreturn.h>

typedef struct s_redirection {
	t_tokentype	type;
	union {
		t_wordlist	wl;
		struct s_heredoc {
			char	**doc;
			char	*file;
		}	hd;
	};
}	t_redir;

bool	redirect(t_redir *redir, int32_t fds[2]);
bool	create_heredoc(t_redir *redir);
bool	read_heredoc(t_redir *redir, char *delim, size_t delim_len);
bool	write_heredoc(t_redir *hd);

#endif //REDIR_H
