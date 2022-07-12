/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/12 16:03:43 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/12 16:03:43 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>
#include <token.h>


bool	build_ast(const char *cmd, t_dynarr *tokens, t_dynarr *trunk)
{
	if (!dynarr_create(trunk, AST_INIT_SIZE, sizeof(t_ast_node)))
		return (false);
	
}