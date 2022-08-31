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

bool	general_error(const char *where);
void	syntax_error_type(t_tokentype type);

static bool	check_start_syntax(t_parser *parser, t_tokentype *type)
{
	t_exp_tok	*token;

	token = dynarr_get(parser->tokens, ++parser->idx);
	*type = token->type;
	if (*type == WORD || *type == PAR_OPEN || is_redir(*type))
		return (false);
	syntax_error_type(*type);
	return (true);
}

t_tokentype	parse_logic(t_parser *parser, t_ast_node **dst, t_tokentype type)
{
	t_tokentype	next_type;

	*dst = init_logic_node(type, *dst);
	if (*dst == NULL)
		return (error_status(NULL, "parse_logic", ERROR));
	if (check_start_syntax(parser, &next_type))
		return (error_status(dst, NULL, SYNTAX));
	next_type = parse_node(parser, next_type, &(*dst)->logic.r);
	if (next_type == PIPE)
		next_type = parse_node(parser, next_type, &(*dst)->logic.r);
	if ((*dst)->logic.r == NULL)
		return (error_status(dst, NULL, 0));
	return (next_type);
}
