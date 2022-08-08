/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_subshell.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/05 19:32:37 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/05 19:32:37 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <execute.h>

uint8_t	execute_subshell(t_paren_node *node, bool must_exit)
{
	uint8_t	status;

	if (!must_exit)
		if (fork_and_wait(&status))
			return (status);
	exit(execute_node(node->contents, true));
}
