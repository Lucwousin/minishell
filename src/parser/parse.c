/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/12 16:03:43 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/12 16:03:43 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>

t_tokentype	parse_node(t_parser *parser, t_tokentype type, t_ast_node **dst)
{
	if (type == PAR_OPEN)
		return (parse_nodelist(parser, dst, true));
	else if (type == WORD || type == VARIABLE || type == GLOB
		|| (type >= RED_IN && type <= RED_APP))
		return (parse_command(parser, dst));
	if (type == PIPE)
		return (parse_pipeline(parser, dst));
	else if (type == OR || type == AND)
		return (parse_logic(parser, dst, type));
	else
		return (-1);
}

t_ast_node	*build_ast(t_dynarr *tokens)
{
	t_parser	parser;
	t_ast_node	*node;

	parser = ((t_parser){0, tokens});
	parse_nodelist(&parser, &node, false);
	return (node);
}
