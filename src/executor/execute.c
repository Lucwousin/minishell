/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/26 20:24:42 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/26 20:24:42 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <execute.h>
#include <minishell.h>
#include <unistd.h>

static const t_executor	g_executors[] = {
[COMMAND] = (t_executor const) execute_command,
[LOGIC] = (t_executor const) execute_logic,
[PARENTHESIS] = (t_executor const) execute_subshell
};

uint8_t	execute_node(t_ast_node *node, bool can_exit)
{
	return (g_executors[node->type](&node->node, can_exit));
}

void	execute(t_ast_node *root_node)
{
	t_paren_node	*root;

	root = &root_node->node.paren;
	if (root->contents == NULL)
		return ;
	g_globals.exit = execute_node(root->contents, false);
}
