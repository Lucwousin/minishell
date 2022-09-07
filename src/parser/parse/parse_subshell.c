/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_subshell.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/19 16:28:22 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/19 16:28:22 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>

void	syntax_error_type(t_tokentype type);

static bool	check_start_syntax(t_parser *parser, bool paren, t_tokentype *type)
{
	parser->idx += paren;
	*type = ((t_token *) parser->tokens->arr)[parser->idx].type;
	if (*type == WORD
		|| is_redir(*type)
		|| *type == PAR_OPEN
		|| (!paren && *type == END_OF_INPUT))
		return (false);
	syntax_error_type(*type);
	return (true);
}

static bool	check_end_syntax(t_tokentype type, bool parenthesis)
{
	if ((type == PAR_CLOSE && parenthesis) || \
		(type == END_OF_INPUT && !parenthesis))
		return (false);
	syntax_error_type(type);
	return (true);
}

static t_tokentype	get_next_type(t_parser *parser, t_tokentype type)
{
	t_token	*next_tok;

	if (type == END_OF_INPUT)
		return (END_OF_INPUT);
	next_tok = dynarr_get(parser->tokens, ++parser->idx);
	return (next_tok->type);
}

static bool	should_parse_next(t_tokentype type)
{
	return (type == OR || type == AND || type == PIPE);
}

t_tokentype	parse_subshell(t_parser *parser, t_ast_node **dst, bool paren)
{
	t_ast_node	**sub;
	t_tokentype	type;

	if (check_start_syntax(parser, paren, &type))
		return (error_status(NULL, NULL, SYNTAX));
	*dst = init_subsh_node();
	if (*dst == NULL)
		return (error_status(NULL, "parse subshell", ERROR));
	sub = &(*dst)->subsh.contents;
	while (true)
	{
		type = parse_node(parser, type, sub);
		if (*sub == NULL)
			return (error_status(dst, NULL, 0));
		if (!should_parse_next(type))
			break ;
	}
	if (check_end_syntax(type, paren))
		return (error_status(dst, NULL, SYNTAX));
	return (get_next_type(parser, type));
}
