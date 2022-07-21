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

bool	malloc_error(const char *where);
void	syntax_error_type(t_tokentype type);

static bool	check_syntax_error(t_exp_tok *token)
{
	t_tokentype	type;

	type = token->type;
	if (type == WORD
		|| type == VARIABLE
		|| type == PAR_OPEN
		|| (type >= RED_IN && type <= RED_APP))
		return (false);
	return (syntax_error_type(type), true);
}

static t_tokentype	finish_node(t_ast_node **dst, t_tokentype type)
{
	t_pipe_node	*p_node;

	p_node = &(*dst)->node.pipe;
	if (!dynarr_finalize(&p_node->nodes))
		return (destroy_node(dst), malloc_error("finish_node (pipe)"));
	return (type);
}

t_tokentype	parse_pipeline(t_parser *parser, t_ast_node **dst)
{
	t_exp_tok	*tok;
	t_ast_node	*sub;
	t_tokentype	next_type;

	*dst = init_pipe_node(*dst);
	if (*dst == NULL)
		return (malloc_error("parse_pipeline"), -1);
	next_type = PIPE;
	while (next_type == PIPE)
	{
		if (parser->idx == parser->tokens->length - 1)
			return (destroy_node(dst), syntax_error_type(END_OF_INPUT), -1);
		tok = dynarr_get(parser->tokens, ++parser->idx);
		if (check_syntax_error(tok))
			return (destroy_node(dst), -1);
		next_type = parse_node(parser, tok->type, &sub);
		if (sub == NULL)
			return (destroy_node(dst), -1);
		if (!dynarr_addone(&(*dst)->node.pipe.nodes, &sub))
			return (malloc_error("parse_pipeline"), -1);
	}
	return (finish_node(dst, next_type));
}