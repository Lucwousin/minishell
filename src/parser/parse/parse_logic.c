/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_logic.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/20 19:47:42 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/20 19:47:42 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>

bool	malloc_error(const char *where);
void	syntax_error_type(t_tokentype type);

static bool	check_start_syntax(t_parser *parser, t_tokentype *type)
{
	t_exp_tok	*token;

	++parser->idx;
	if (parser->idx >= parser->tokens->length)
		return (syntax_error_type(END_OF_INPUT), true);
	token = dynarr_get(parser->tokens, parser->idx);
	*type = token->type;
	if (*type == WORD
		|| *type == VARIABLE
		|| *type == PAR_OPEN
		|| (*type >= RED_IN && *type <= RED_APP))
		return (false);
	return (syntax_error_type(*type), true);
}

static bool	is_terminating_type(t_tokentype type)
{
	return (type == END_OF_INPUT
		|| type == PAR_OPEN
		|| type == PAR_CLOSE
		|| type == AND
		|| type == OR);
}

t_tokentype	parse_logic(t_parser *parser, t_ast_node **dst, t_tokentype type)
{
	t_ast_node	*r;
	t_tokentype	next_type;

	*dst = init_logic_node(type, *dst);
	if (*dst == NULL)
		return (malloc_error("parse_logic"), -1);
	if (check_start_syntax(parser, &next_type))
		return (destroy_node(dst), -1);
	while (parser->idx < parser->tokens->length)
	{
		next_type = parse_node(parser, next_type, &r);
		if (r == NULL)
			return (destroy_node(dst), -1);
		if (is_terminating_type(next_type))
			break ;
	}
	(*dst)->node.logic.r = r;
	if (next_type == PAR_OPEN)
		return (syntax_error_type(next_type), destroy_node(dst), -1);
	return (next_type);
}
