/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   destroy_node.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/26 15:11:33 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/26 15:11:33 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>
#include <stdlib.h>

static void	destroy_command(t_ast_node *node)
{
	t_cmd_node	*c_node;

	c_node = &node->node.command;
	dynarr_delete(&c_node->argv);
	dynarr_delete(&c_node->redirs);
}

static void	destroy_logic(t_ast_node *node)
{
	t_logic_node	*l_node;

	l_node = &node->node.logic;
	destroy_node(&l_node->l);
	destroy_node(&l_node->r);
}

static void	destroy_pipeline(t_ast_node *node)
{
	t_pipe_node	*p_node;
	t_ast_node	**nodes;
	size_t		i;

	p_node = &node->node.pipe;
	nodes = p_node->nodes.arr;
	i = p_node->nodes.length;
	while (i--)
		destroy_node(nodes + i);
	dynarr_delete(&p_node->nodes);
}

static void	destroy_parenthesis(t_ast_node *node)
{
	t_paren_node	*p_node;

	p_node = &node->node.paren;
	destroy_node(&p_node->contents);
}

static void	(*g_destroylut[])(t_ast_node *) = {
[COMMAND] = destroy_command,
[LOGIC] = destroy_logic,
[PIPELINE] = destroy_pipeline,
[PARENTHESIS] = destroy_parenthesis
};

void	destroy_node(t_ast_node **nodep)
{
	if (*nodep == NULL)
		return ;
	g_destroylut[(*nodep)->type](*nodep);
	free(*nodep);
	*nodep = NULL;
	return ;
}
