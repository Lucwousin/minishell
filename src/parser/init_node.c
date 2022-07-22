/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_node.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/19 15:55:33 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/19 15:55:33 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>
#include <stdlib.h>

t_ast_node	*init_cmd_node(void)
{
	t_ast_node	*node;
	t_cmd_node	*c_node;

	node = malloc(sizeof(t_ast_node));
	if (node == NULL)
		return (NULL);
	node->type = COMMAND;
	c_node = &node->node.command;
	if (!dynarr_create(&c_node->argv, ARGV_INIT_SIZE, sizeof(char **)))
		return (free(node), NULL);
	if (!dynarr_create(&c_node->redirs, REDIR_INIT_SIZE, sizeof(t_redir)))
		return (dynarr_delete(&c_node->argv), free(node), NULL);
	return (node);
}

t_ast_node	*init_paren_node(void)
{
	t_ast_node		*node;

	node = malloc(sizeof(t_ast_node));
	if (node == NULL)
		return (NULL);
	node->type = PARENTHESIS;
	node->node.paren.contents = NULL;
	return (node);
}

t_ast_node	*init_pipe_node(t_ast_node *first)
{
	t_ast_node	*node;
	t_pipe_node	*p_node;

	node = malloc(sizeof(t_ast_node));
	if (node == NULL)
		return (NULL);
	node->type = PIPELINE;
	p_node = &node->node.pipe;
	if (!dynarr_create(&p_node->nodes, PIPE_INIT_SIZE, sizeof(t_ast_node *)))
		return (free(node), NULL);
	if (dynarr_addone(&p_node->nodes, &first))
		return (node);
	dynarr_delete(&p_node->nodes);
	free(node);
	return (NULL);
}

t_ast_node	*init_logic_node(t_tokentype type, t_ast_node *left)
{
	t_ast_node		*node;
	t_logic_node	*l_node;

	node = malloc(sizeof(t_ast_node));
	if (node == NULL)
		return (destroy_node(&left), NULL);
	node->type = LOGICAL_EXPRESSION;
	l_node = &node->node.logic;
	l_node->type = type;
	l_node->l = left;
	l_node->r = NULL;
	return (node);
}
