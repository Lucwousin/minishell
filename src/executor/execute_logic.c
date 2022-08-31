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

static bool	should_exe_next(t_tokentype type, uint8_t status)
{
	if (status != EXIT_SUCCESS)
		return (false);
	if (type == OR)
		return (g_globals.exit != 0);
	else
		return (g_globals.exit == 0);
}

uint8_t	execute_logic(t_ast_node *node, bool must_exit)
{
	uint8_t	status;

	status = execute_node(node->logic.l, false);
	if (should_exe_next(node->logic.type, status))
		status = execute_node(node->logic.r, must_exit);
	return (try_exit(status, must_exit));
}
