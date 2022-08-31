/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_pipeline.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/20 17:45:23 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/20 17:45:23 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>

bool	general_error(const char *where);
void	syntax_error_type(t_tokentype type);

static bool	add_node(t_ast_node *node, t_ast_node **sub)
{
	if (*sub == NULL)
		return (false);
	if (dynarr_addone(&node->pipe.nodes, sub))
		return (true);
	general_error("parse_pipeline add_node");
	destroy_node(sub);
	return (false);
}

t_tokentype	parse_pipeline(t_parser *parser, t_ast_node **dst)
{
	t_ast_node	*sub;
	t_tokentype	type;

	*dst = init_pipe_node(*dst);
	if (*dst == NULL)
		return (error_status(NULL, "parse_pipeline", ERROR));
	type = PIPE;
	while (type == PIPE)
	{
		type = ((t_token *) dynarr_get(parser->tokens, ++parser->idx))->type;
		if (type != WORD && type != PAR_OPEN && !is_redir(type))
			return (syntax_err(dst, type));
		type = parse_node(parser, type, &sub);
		if (!add_node(*dst, &sub))
			return (error_status(dst, NULL, 0));
	}
	return (type);
}
