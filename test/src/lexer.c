/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/07 22:09:07 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/07 22:09:07 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <get_next_line.h>
#include <token.h>
#include <stdio.h>
#include <unistd.h>
#include <minishell.h>

t_sh_env	g_globals = {.exit = 0};

static const char	*g_tokenstr[] = {
[END_OF_INPUT] = "END_OF_INPUT",
[WHITESPACE] = "WHITESPACE",
[WORD] = "WORD",
[VARIABLE] = "VARIABLE",
[SQUOTE] = "SQUOTE",
[DQUOTE] = "DQUOTE",
[PAR_OPEN] = "PAR_OPEN",
[PAR_CLOSE] = "PAR_CLOSE",
[OPERATOR] = "OPERATOR",
};

static void	print_token(void *tokenp, void *str)
{
	t_token	*token;
	char	*substr;

	token = tokenp;
	substr = ft_substr(str, token->start, token->end - token->start + 1);
	printf("%14s - start %4lu - end %4lu \"%s\"\n",
		g_tokenstr[token->type], token->start, token->end, substr);
	free(substr);
}

static void	hook(t_in_handler *h)
{
	if (h->state == TOKENIZE)
		printf("%s - %lu tokens\n", h->input, h->tokens.length);
	else
		printf("after whitespace removal - %lu tokens\n", h->tokens.length);
	dynarr_foreach(&h->tokens, print_token, (void *) h->input);
}

static void	test(char *line)
{
	t_in_handler	handler;
	uint8_t			status;

	line[ft_strlen(line) - 1] = '\0';
	init_handler(&handler, line);
	handler.hooks[TOKENIZE] = hook;
	handler.hooks[EVALUATE] = hook;
	status = handle_input_target(&handler, EVALUATE);
	if (status != SUCCESS)
		printf("handle input exited with status %u for %s\n", status, line);
	else
		clean_handler(&handler, status);
}

int	main(void)
{
	char	*line;

	while (true)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break ;
		if (*line != '#')
			test(line);
		else if (line[1] != '\n')
			printf("\n%s", line + 1);
		free(line);
	}
}
