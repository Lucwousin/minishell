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
#include <redir.h>
#include <stdlib.h>

t_ast_node	*init_cmd_node(void)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (node == NULL)
		return (NULL);
	node->type = COMMAND;
	node->command.args.next = NULL;
	node->command.args.word = (void *) &node->command.args;
	if (dynarr_create(&node->command.redirs, REDIR_INIT_SIZE, sizeof(t_redir)))
		return (node);
	free(node);
	return (NULL);
}

t_ast_node	*init_paren_node(void)
{
	t_ast_node		*node;

	node = malloc(sizeof(t_ast_node));
	if (node == NULL)
		return (NULL);
	node->type = PARENTHESIS;
	node->paren.contents = NULL;
	return (node);
}

t_ast_node	*init_pipe_node(t_ast_node *first)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (node == NULL)
	{
		destroy_node(&first);
		return (NULL);
	}
	node->type = PIPELINE;
	if (dynarr_create(&node->pipe.nodes, PIPE_INIT_SIZE, sizeof(t_ast_node *)))
	{
		if (dynarr_addone(&node->pipe.nodes, &first))
			return (node);
		dynarr_delete(&node->pipe.nodes);
	}
	destroy_node(&first);
	free(node);
	return (NULL);
}

t_ast_node	*init_logic_node(t_tokentype type, t_ast_node *left)
{
	t_ast_node		*node;

	node = malloc(sizeof(t_ast_node));
	if (node == NULL)
	{
		destroy_node(&left);
		return (NULL);
	}
	node->type = LOGIC;
	node->logic.type = type;
	node->logic.l = left;
	node->logic.r = NULL;
	return (node);
}
