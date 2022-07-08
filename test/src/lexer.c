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

#include "get_next_line.h"
#include "dynarr.h"
#include "token.h"
#include <stdio.h>
#include <unistd.h>

static const char	*g_tokenstr[] = {
[END_OF_INPUT] = "END_OF_INPUT",
[WORD] = "WORD",
[VARIABLE] = "VARIABLE",
[QUOTE] = "QUOTE",
[OPERATOR] = "OPERATOR",
[PIPE] = "PIPE",
[RED_IN] = "RED_IN",
[RED_HD] = "RED_HD",
[RED_OUT] = "RED_OUT",
[RED_APP] = "RED_APP",
};

static void	print_token(void *tokenp, void *ign)
{
	t_token	*token = tokenp;

	(void) ign;
	printf("token %s - start %lu - end %lu ", 
		g_tokenstr[token->token], token->start, token->end);
	if ((token->token == WORD || token->token == QUOTE)
		&& token->sub.length != 0)
	{
		printf("subtokens:\n");
		dynarr_foreach(&token->sub, print_token, "\t");
	}
	printf("token end\n");
}

static void	test(char *line)
{
	t_dynarr	output;
	size_t		sub_depth;

	sub_depth = 0;
	tokenize(&output, line);
	printf("%s - %lu tokens\n", line, output.length);
	dynarr_foreach(&output, print_token, &sub_depth);
}

int	main(void)
{
	char	*line;

	while ((line = get_next_line(STDIN_FILENO)))
	{
		if (*line != '#')
			test(line);
		else
			printf("%s\n", line + 1);
		free(line);
	}
}