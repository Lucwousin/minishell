/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_command.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/20 17:24:00 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/20 17:24:00 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>
#include <redir.h>

bool	malloc_error(const char *where);
void	syntax_error_type(t_tokentype type);

static bool	add_redir(t_cmd_node *cmd, t_exp_tok *tok)
{
	t_redir	red;

	red.type = tok->type;
	red.str = tok->str;
	if (red.type == RED_HD || red.type == RED_HD_Q)
		if (!create_heredoc(&red.str, red.type == RED_HD))
			return (false);
	if (!dynarr_addone(&cmd->redirs, &red))
		return (malloc_error("add_redir"));
	return (true);
}

static bool	is_terminating_type(t_tokentype type)
{
	return (type == PAR_CLOSE || type == AND || type == OR || type == PIPE);
}

static t_tokentype	finish_node(t_ast_node **dst, t_tokentype type)
{
	static const void	*null = NULL;
	t_cmd_node			*c_node;

	c_node = &(*dst)->node.command;
	if (!dynarr_addone(&c_node->argv, &null) || \
		!dynarr_finalize(&c_node->argv) || \
		!dynarr_finalize(&c_node->redirs))
		return (destroy_node(dst), malloc_error("finish_node (command)"));
	return (type);
}

t_tokentype	parse_command(t_parser *parser, t_ast_node **dst)
{
	t_exp_tok	*tok;

	*dst = init_cmd_node();
	if (*dst == NULL)
		return (malloc_error("parse_command"), -1);
	tok = NULL;
	while (parser->idx < parser->tokens->length)
	{
		tok = dynarr_get(parser->tokens, parser->idx);
		if (is_terminating_type(tok->type))
			return (finish_node(dst, tok->type));
		if (tok->type == PAR_OPEN)
			return (syntax_error_type(PAR_OPEN), -1);
		if (tok->type == WORD || tok->type == VARIABLE || tok->type == GLOB)
			if (!dynarr_addone(&(*dst)->node.command.argv, &tok->str))
				return (destroy_node(dst), malloc_error("parse_command"), -1);
		if (tok->type >= RED_IN && tok->type <= RED_APP)
			if (!add_redir(&(*dst)->node.command, tok))
				return (destroy_node(dst), -1);
		++parser->idx;
	}
	return (finish_node(dst, END_OF_INPUT));
}
