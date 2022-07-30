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
#include <unistd.h>
#include <sysexits.h>
#include <stdlib.h>

static pid_t	execute_node(t_ast_node *node, int32_t in, int32_t out)
{
	pid_t	pid;

	pid = fork();
	if (pid != 0)
		return (pid);
	if (in != STDIN_FILENO)
		if (dup2(in, STDIN_FILENO) == -1)
			exit(EXIT_FAILURE);
	if (out != STDOUT_FILENO)
		if (dup2(out, STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
	if (node->type == COMMAND)
		execute_command(node);
	if (node->type == PIPELINE)
		execute_pipeline(node);
	if (node->type == PARENTHESIS)
		execute_parenthesis(node);
	if (node->type == LOGICAL_EXPRESSION)
		execute_logic(node);
}

void	execute(t_ast_node *root_node, int32_t *exit)
{
	t_paren_node	*root;
	pid_t			pid;

	root = &root_node->node.paren;
	if (root->contents == NULL)
		return ;
	pid = execute_node(root->contents, STDIN_FILENO, STDOUT_FILENO);
}