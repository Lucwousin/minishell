/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_command.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/20 17:24:00 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/20 17:24:00 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>

bool	general_error(const char *where);
void	syntax_error_type(t_tokentype type);

static bool	check_start_syntax(t_parser *parser, t_tokentype *type)
{
	*type = ((t_token *) parser->tokens->arr)[parser->idx].type;
	if (*type == WORD || is_redir(*type))
		return (false);
	syntax_error_type(*type);
	return (true);
}

static bool	add_arg(t_parser *parser, t_ast_node *dst)
{
	t_token		*token;
	t_wordlist	*word;
	t_wordlist	*last;

	token = dynarr_get(parser->tokens, parser->idx++);
	word = new_wordlist_token(parser->cmd, token);
	if (word == NULL)
		return (false);
	last = (void *) dst->command.args.word;
	last->next = word;
	dst->command.args.word = (void *) word;
	return (true);
}

t_tokentype	parse_command(t_parser *parser, t_ast_node **dst)
{
	t_tokentype	type;

	if (check_start_syntax(parser, &type))
		return (error_status(NULL, NULL, SYNTAX));
	*dst = init_cmd_node();
	if (*dst == NULL)
		return (error_status(NULL, "parse_cmd", ERROR));
	while (type == WORD || is_redir(type))
	{
		if (type == WORD)
			if (!add_arg(parser, *dst))
				return (error_status(dst, "parse_cmd", ERROR));
		if (is_redir(type))
			if (!parse_redir(parser, dst))
				return (error_status(dst, NULL, 0));
		type = ((t_token *) dynarr_get(parser->tokens, parser->idx))->type;
	}
	return (type);
}
