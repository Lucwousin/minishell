/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   preparser.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/15 21:48:40 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/15 21:48:40 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <get_next_line.h>
#include <token.h>
#include <unistd.h>
#include <stdio.h>
#include <minishell.h>
#include <environ.h>

#define EVAL_MSG	"%s - %lu tokens after whitespace removal\n"
#define EXPAND_MSG	"%lu tokens after expansion/concatenation\n"

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
[RED_IN] = "RED_IN",
[RED_HD] = "RED_HD",
[RED_HD_Q] = "RED_HD_Q",
[RED_OUT] = "RED_OUT",
[RED_APP] = "RED_APP",
[OR] = "OR",
[AND] = "AND",
[PIPE] = "PIPE",
};

t_sh_env	g_globals = {.exit = 0};

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

static void	print_exp_token(void *p, void *ign)
{
	t_exp_tok	*t;

	(void) ign;
	t = p;
	printf("%14s - \"%s\"\n", g_tokenstr[t->type], t->str);
}

static void	hook(t_in_handler *handler)
{
	if (handler->state == EVALUATE)
	{
		printf(EVAL_MSG, handler->input, handler->tokens.length);
		dynarr_foreach(&handler->tokens, print_token, (void *)handler->input);
	}
	else
	{
		printf(EXPAND_MSG, handler->expanded_tokens.length);
		dynarr_foreach(&handler->expanded_tokens, print_exp_token, NULL);
	}
}

static void	test(char *line)
{
	t_in_handler	handler;
	uint8_t			status;

	init_handler(&handler, line);
	handler.hooks[EVALUATE] = hook;
	handler.hooks[PREPARSE] = hook;
	status = handle_input_target(&handler, PREPARSE);
	if (status != EXIT_SUCCESS)
		printf("handle input exited with status %u for %s\n", status, line);
	else
		clean_handler(&handler, status);
}

int	main(void)
{
	char	*line;

	init_environment();
	while (true)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break ;
		line[ft_strlen(line) - 1] = '\0';
		if (*line == '%')
			set_variable(line + 1, var_name_end(line + 1));
		else if (*line != '#')
			test(line);
		else
			printf("\n%s\n", line + 1);
		free(line);
	}
	clean_environment();
}
