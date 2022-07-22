/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_node.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/12 16:03:43 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/12 16:03:43 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>
#include <token.h>
#include <stdlib.h>

static void	destroy_node_arg(void *nodep, void *ign)
{
	(void) ign;
	destroy_node(nodep);
}

t_tokentype	parse_node(t_parser *parser, t_tokentype type, t_ast_node **dst)
{
	if (type == PAR_OPEN)
		return (parse_nodelist(parser, dst, true));
	else if (type == WORD || type == VARIABLE
		|| (type >= RED_IN && type <= RED_APP))
		return (parse_command(parser, dst));
	if (type == PIPE)
		return (parse_pipeline(parser, dst));
	else if (type == OR || type == AND)
		return (parse_logic(parser, dst, type));
	else
		return (-1);
}

void	destroy_node(t_ast_node **nodep)
{
	t_ast_node	*node;

	if (*nodep == NULL)
		return ;
	node = *nodep;
	if (node->type == COMMAND)
	{
		dynarr_delete(&node->node.command.argv);
		dynarr_delete(&node->node.command.redirs);
	}
	else if (node->type == LOGICAL_EXPRESSION)
	{
		destroy_node(&node->node.logic.l);
		destroy_node(&node->node.logic.r);
	}
	else if (node->type == PIPELINE)
	{
		dynarr_foreach(&node->node.pipe.nodes, destroy_node_arg, NULL);
		dynarr_delete(&node->node.pipe.nodes);
	}
	else
	{
		destroy_node(&node->node.paren.contents);
	}
	free(node);
	*nodep = NULL;
}

t_ast_node	*build_ast(t_dynarr *tokens)
{
	t_parser	parser;
	t_ast_node	*node;

	parser = ((t_parser){0, tokens});
	parse_nodelist(&parser, &node, false);
	return (node);
}
