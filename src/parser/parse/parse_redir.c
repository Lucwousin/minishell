/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_redir.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/25 17:42:48 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/25 17:42:48 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>
#include <redir.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

static bool	check_syntax(t_parser *parser, t_ast_node **dst)
{
	const t_token	*token = dynarr_get(parser->tokens, parser->idx);

	if (token->type == WORD)
		return (false);
	syntax_err(dst, token->type);
	return (true);
}

static bool	error(t_redir *red, bool has_list)
{
	perror("parse_redir");
	if (has_list)
		destroy_wordlist(&red->wl);
	else
	{
		free(red->hd.file);
		ft_free_mult((void **) red->hd.doc);
	}
	return (false);
}

bool	parse_redir(t_parser *parser, t_ast_node **dst)
{
	const t_token	*tokens = parser->tokens->arr;
	t_wordlist		*cur;
	t_redir			redir;

	redir.type = tokens[parser->idx++].type;
	if (check_syntax(parser, dst))
		return (false);
	cur = &redir.wl;
	while (tokens[parser->idx].flags & REDIR_WORD)
	{
		cur->next = new_wordlist_token(parser->cmd, tokens + parser->idx++);
		if (cur->next == NULL)
			return (error(&redir, true));
		if (redir.type == RED_HD && cur->next->flags & IS_QUOTED)
			redir.type = RED_HD_Q;
		cur = cur->next;
	}
	if (redir.type == RED_HD || redir.type == RED_HD_Q)
		if (!create_heredoc(&redir))
			return (false);
	if (!dynarr_addone(&(*dst)->command.redirs, &redir))
		return (error(&redir, redir.type != RED_HD && redir.type != RED_HD_Q));
	return (true);
}
