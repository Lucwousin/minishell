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
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define RES_DIR "test/res"
#define TEST_NAME "lexer"

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
	printf("token %s - start %lu - end %lu\n",
			g_tokenstr[token->token], token->start, token->end);
	if ((token->token == WORD || token->token == QUOTE)
		&& token->sub.length != 0)
	{
		printf("subtokens:\n");
		dynarr_foreach(&token->sub, print_token, ign);
	}
	printf("token end\n\n");
}

static void	test(char *line)
{
	t_dynarr	output;

	tokenize(&output, line);
	printf("%s - %lu tokens\n", line, output.length);
	dynarr_foreach(&output, print_token, NULL);
}

int	main(void)
{
	char	*line;
	int32_t	fd;

	fd = open(RES_DIR"/"TEST_NAME, O_RDONLY);

	while ((line = get_next_line(fd)))
	{
		test(line);
		free(line);
	}
}