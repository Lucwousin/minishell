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
#include <dynarr.h>
#include <token.h>
#include <stdio.h>
#include <unistd.h>

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
	printf("token %14s - start %4lu - end %4lu \"%s\"\n",
		g_tokenstr[token->token], token->start, token->end, substr);
	free(substr);
}

static void	test(char *line)
{
	t_dynarr	output;

	line[ft_strlen(line) - 1] = '\0';
	tokenize(&output, line);
	printf("%s - %lu tokens\n", line, output.length);
	dynarr_foreach(&output, print_token, line);
	evaluate(&output);
	printf("after whitespace removal - %lu tokens\n", output.length);
	dynarr_foreach(&output, print_token, line);
	dynarr_delete(&output);
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
		else
			printf("\n%s", line + 1);
		free(line);
	}
}
