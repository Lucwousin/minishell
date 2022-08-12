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

bool	general_error(const char *where);
void	syntax_error_type(t_tokentype type);

static uint8_t	syntax_err(t_ast_node **dst, t_tokentype type)
{
	syntax_error_type(type);
	return (error_status(dst, NULL, SYNTAX));
}

static bool	add_redir(t_cmd_node *cmd, t_exp_tok *tok)
{
	t_redir	red;

	if (tok->type == RED_HD || tok->type == RED_HD_Q)
		if (!create_heredoc(&tok->str, tok->type == RED_HD))
			return (false);
	red.type = tok->type;
	red.str = tok->str;
	if (!dynarr_addone(&cmd->redirs, &red))
		return (general_error("add_redir"));
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
	if (!dynarr_addone(&c_node->argv, &null))
		return (error_status(dst, "finish_node (command)", ERROR));
	return (type);
}

t_tokentype	parse_command(t_parser *parser, t_ast_node **dst)
{
	t_exp_tok	*tok;

	*dst = init_cmd_node();
	if (*dst == NULL)
		return (error_status(NULL, "parse_cmd", ERROR));
	tok = NULL;
	while (parser->idx < parser->tokens->length)
	{
		tok = dynarr_get(parser->tokens, parser->idx);
		if (is_terminating_type(tok->type))
			return (finish_node(dst, tok->type));
		if (tok->type == PAR_OPEN)
			return (syntax_err(dst, PAR_OPEN));
		if (tok->type == WORD || tok->type == VARIABLE || tok->type == GLOB)
			if (!dynarr_addone(&(*dst)->node.command.argv, &tok->str))
				return (error_status(dst, "parse_cmd", ERROR));
		if (tok->type >= RED_IN && tok->type <= RED_APP)
			if (!add_redir(&(*dst)->node.command, tok))
				return (error_status(dst, NULL, 0));
		++parser->idx;
	}
	return (finish_node(dst, END_OF_INPUT));
}
