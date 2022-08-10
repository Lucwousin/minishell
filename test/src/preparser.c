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
	free(t->str);
}

static void	test(char *line)
{
	t_dynarr	tokens;
	t_dynarr	exp_tokens;

	tokenize(&tokens, line);
	evaluate(&tokens);
	printf("%s after whitespace removal: %lu tokens\n", line, tokens.length);
	dynarr_foreach(&tokens, print_token, line);
	if (preparse(line, &tokens, &exp_tokens))
		printf("Syntax error!\n");
	else
	{
		printf("After pre-parsing - %lu tokens\n", exp_tokens.length);
		dynarr_foreach(&exp_tokens, print_exp_token, NULL);
		dynarr_delete(&exp_tokens);
	}
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
