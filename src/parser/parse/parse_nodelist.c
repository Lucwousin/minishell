/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_nodelist.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/19 16:28:22 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/19 16:28:22 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>

bool	malloc_error(const char *where);
void	syntax_error_type(t_tokentype type);

static bool	check_start_syntax(t_parser *parser, bool paren, t_tokentype *type)
{
	if (paren)
		++parser->idx;
	if (parser->idx >= parser->tokens->length)
		return (syntax_error_type(END_OF_INPUT), true);
	*type = ((t_exp_tok *) dynarr_get(parser->tokens, parser->idx))->type;
	if (*type == WORD
		|| *type == VARIABLE
		|| *type == PAR_OPEN
		|| *type == GLOB
		|| (*type >= RED_IN && *type <= RED_APP))
		return (false);
	return (syntax_error_type(*type), true);
}

static bool	check_end_syntax(t_tokentype type, bool parenthesis)
{
	if (!(type == PAR_CLOSE && !parenthesis) && \
		!(type == END_OF_INPUT && parenthesis))
		return (false);
	return (syntax_error_type(type), true);
}

static t_tokentype	get_next_type(t_parser *parser, t_tokentype type)
{
	t_exp_tok	*next_tok;

	if (type == END_OF_INPUT || parser->idx >= parser->tokens->length - 1)
		return (END_OF_INPUT);
	next_tok = dynarr_get(parser->tokens, ++parser->idx);
	return (next_tok->type);
}

t_tokentype	parse_nodelist(t_parser *parser, t_ast_node **dst, bool paren)
{
	t_ast_node	*sub;
	t_tokentype	next_type;

	t_exp_tok	*arr = parser->tokens->arr;
	if (arr == NULL)
		return -1;
	*dst = init_paren_node();
	if (*dst == NULL)
		return (malloc_error("parse_nodelist"), -1);
	if (check_start_syntax(parser, paren, &next_type))
		return (destroy_node(dst), -1);
	while (parser->idx < parser->tokens->length)
	{
		next_type = parse_node(parser, next_type, &sub);
		if (sub == NULL)
			return (destroy_node(dst), -1);
		if (check_end_syntax(next_type, paren))
			return (destroy_node(dst), -1);
		if (next_type == PAR_CLOSE || next_type == END_OF_INPUT)
			break ;
	}
	(*dst)->node.paren.contents = sub;
	next_type = get_next_type(parser, next_type);
	return (next_type);
}
