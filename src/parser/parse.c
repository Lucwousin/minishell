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

void	syntax_error_type(t_tokentype type);

uint8_t	error_status(t_ast_node **node, char *where, uint8_t status)
{
	if (status == ERROR && where != NULL)
		perror(where);
	if (node != NULL)
		destroy_node(node);
	if (status != SUCCESS)
		g_env.exit = status;
	return (-1);
}

uint8_t	syntax_err(t_ast_node **dst, t_tokentype type)
{
	syntax_error_type(type);
	return (error_status(dst, NULL, SYNTAX));
}

t_tokentype	parse_node(t_parser *parser, t_tokentype type, t_ast_node **dst)
{
	if (type == PAR_OPEN)
		return (parse_subshell(parser, dst, true));
	else if (type == WORD || is_redir(type))
		return (parse_command(parser, dst));
	if (type == PIPE)
		return (parse_pipeline(parser, dst));
	else if (type == OR || type == AND)
		return (parse_logic(parser, dst, type));
	syntax_error_type(type);
	return (error_status(dst, NULL, SYNTAX));
}

uint8_t	build_ast(const char *cmd, t_dynarr *tokens, t_ast_node **dst)
{
	t_parser	parser;

	if (tokens->length == 1)
		return (SUCCESS);
	parser = ((t_parser){cmd, 0, tokens});
	if (parse_subshell(&parser, dst, false) != END_OF_INPUT)
		return (ERROR);
	return (SUCCESS);
}
