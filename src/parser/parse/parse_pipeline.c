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

static uint8_t	syntax_err(t_ast_node **dst, t_tokentype type)
{
	syntax_error_type(type);
	return (error_status(dst, NULL, SYNTAX));
}

static bool	check_syntax_error(t_exp_tok *token)
{
	t_tokentype	type;

	type = token->type;
	if (type == WORD
		|| type == VARIABLE
		|| type == PAR_OPEN
		|| type == GLOB
		|| (type >= RED_IN && type <= RED_APP))
		return (false);
	return (syntax_error_type(type), true);
}

static t_tokentype	finish_node(t_ast_node **dst, t_tokentype type)
{
	t_pipe_node	*p_node;

	p_node = &(*dst)->node.pipe;
	if (!dynarr_finalize(&p_node->nodes))
		return (error_status(dst, "finish_node (pipe)", ERROR));
	return (type);
}

t_tokentype	parse_pipeline(t_parser *parser, t_ast_node **dst)
{
	t_exp_tok	*tok;
	t_ast_node	*sub;
	t_tokentype	next_type;

	*dst = init_pipe_node(*dst);
	if (*dst == NULL)
		return (error_status(NULL, "parse_pipeline", ERROR));
	next_type = PIPE;
	while (next_type == PIPE)
	{
		if (parser->idx == parser->tokens->length - 1)
			return (syntax_err(dst, END_OF_INPUT));
		tok = dynarr_get(parser->tokens, ++parser->idx);
		if (check_syntax_error(tok))
			return (error_status(dst, NULL, SYNTAX));
		next_type = parse_node(parser, tok->type, &sub);
		if (sub == NULL)
			return (error_status(dst, NULL, 0));
		if (!dynarr_addone(&(*dst)->node.pipe.nodes, &sub))
			return (error_status(dst, "parse_pipeline", ERROR));
	}
	return (finish_node(dst, next_type));
}
