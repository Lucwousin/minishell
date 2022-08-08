/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/12 16:03:43 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/12 16:03:43 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>
#include <stdio.h>
#include <minishell.h>

bool	general_error(const char *where);

uint8_t	error_status(t_ast_node **node, char *where, uint8_t status)
{
	if (status == ERROR && where != NULL)
		perror(where);
	if (node != NULL)
		destroy_node(node);
	if (status != SUCCESS)
		g_globals.exit = status;
	return (-1);
}

t_tokentype	parse_node(t_parser *parser, t_tokentype type, t_ast_node **dst)
{
	if (type == PAR_OPEN)
		return (parse_nodelist(parser, dst, true));
	else if (type == WORD || type == VARIABLE || type == GLOB
		|| (type >= RED_IN && type <= RED_APP))
		return (parse_command(parser, dst));
	if (type == PIPE)
		return (parse_pipeline(parser, dst));
	else if (type == OR || type == AND)
		return (parse_logic(parser, dst, type));
	else
		return (-1);
}

uint8_t	build_ast(t_dynarr *tokens, t_ast_node **dst)
{
	t_parser	parser;
	uint8_t		status;

	parser = ((t_parser){0, tokens});
	status = SUCCESS;
	if (tokens->length == 0)
		dynarr_delete(tokens);
	else
		status = parse_nodelist(&parser, dst, false);
	return (status);
}
