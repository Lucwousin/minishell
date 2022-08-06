/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_logic.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/05 19:32:37 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/05 19:32:37 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <execute.h>

uint8_t	execute_logic(t_logic_node *node, bool can_exit)
{
	uint8_t	status;

	status = execute_node(node->l, false);
	if ((node->type == OR && status) || \
		(node->type == AND && !status))
		status = execute_node(node->r, can_exit);
	return (finish_leaf(status, can_exit));
}
