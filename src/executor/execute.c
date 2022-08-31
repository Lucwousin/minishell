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

static const t_executor	g_executors[] = {
[COMMAND] = (t_executor const) execute_command,
[LOGIC] = (t_executor const) execute_logic,
[PARENTHESIS] = (t_executor const) execute_subshell,
[PIPELINE] = (t_executor const) execute_pipeline
};

uint8_t	execute_node(t_ast_node *node, bool must_exit)
{
	return (g_executors[node->type](node, must_exit));
}

uint8_t	execute(t_ast_node *root_node)
{
	t_paren_node	*root;

	if (root_node == NULL)
		return (EXIT_SUCCESS);
	root = &root_node->paren;
	if (root->contents == NULL)
		return (EXIT_SUCCESS);
	return (execute_node(root->contents, false));
}
