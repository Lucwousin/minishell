#include "test.h"
#include "get_next_line.h"
#include "input.h"
#include <minishell.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

const char	*e_in_states[] = {
[TOKENIZE] = "TOKENIZE",
[PARSE] = "PARSE",
[EXECUTE] = "EXECUTE",
};

static t_hstate	get_target(char **line)
{
	for (t_hstate i = TOKENIZE; i < DONE; i++)
	{
		size_t	len = strlen(e_in_states[i]);
		if (strncmp(*line, e_in_states[i], len) != 0)
			continue ;
		*line += len + 1;
		return (i);
	}
	return (EXECUTE);
}

static void	tokenize_hook(t_in_handler *h)
{
	fflush(stdout);
	printf("Tokenize:\n");
	print_tokens(h->input, dynarr_get(&h->tokens, 0), dynarr_get(&h->tokens, h->tokens.length - 1));
	printf("\n");
	fflush(stdout);
}

static void	parse_hook(t_in_handler *h)
{
	fflush(stdout);
	printf("Parse:\n");
	if (h->root_node == NULL)
		printf("\tRoot node is NULL!\n");
	else
		print_node(h->input, h->root_node, 0);
	printf("\n");
	fflush(stdout);
}

static void	test(char *line)
{
	t_in_handler	handler;
	t_hstate		target;
	uint8_t			status;

	target = get_target(&line);
	init_handler(&handler, line);
	printf("Handling `%s' up to state %s\n", line, e_in_states[target]);
	handler.hooks[TOKENIZE] = tokenize_hook;
	handler.hooks[PARSE] = parse_hook;
	status = handle_input_target(&handler, target);
	printf("`%s' done: status %u\n\n", line, g_globals.exit);
	clean_handler(&handler, status);
}

void	perform_test(void)
{
	char	*line;

	while (true)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			exit(0);
		fflush(stdout);
		line[ft_strlen(line) - 1] = '\0';
		if (*line != '#')
			test(line);
		else
			printf("%s\n", line + 1);
		free(line);
		fflush(stdout);
	}
}
