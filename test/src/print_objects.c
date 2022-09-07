#include <ms_types.h>
#include <stdio.h>
#include "libft.h"
#include "redir.h"

static const char	*g_token_type_names[] = {
[NO_TYPE] = "NO_TYPE",
[WHITESPACE] = "WHITESPACE",
[WORD] = "WORD",
[VARIABLE] = "VARIABLE",
[SQUOTE] = "SQUOTE",
[DQUOTE] = "DQUOTE",
[OPERATOR] = "OPERATOR",
[RED_IN] = "RED_IN",
[RED_HD] = "RED_HD",
[RED_HD_Q] = "RED_HD_Q",
[RED_OUT] = "RED_OUT",
[RED_APP] = "RED_APP",
[PAR_OPEN] = "PAR_OPEN",
[PAR_CLOSE] = "PAR_CLOSE",
[OR] = "OR",
[AND] = "AND",
[PIPE] = "PIPE",
[END_OF_INPUT] = "END_OF_INPUT",
};

static char *get_flags(t_flag flags)
{
	static char	str[7];

	for (int i = 5; i >= 0; i--)
	{
		str[i] = flags & 1 ? '1' : '0';
		flags >>= 1;
	}
	return (str);
}

void	print_tokens(const char *cmd, t_token *start, t_token *end)
{
	printf("\tToken list (%lu)\n", end - start + 1);
	do {
		char	*str = ft_substr(cmd, start->start, start->end - start->start + 1);
		printf("\t\t%-12s - %s - (%s)\n", g_token_type_names[start->type], str, get_flags(start->flags));
		free(str);
	} while (start++ != end);
	printf("\tToken list end\n");
}

static void indent(uint32_t indent)
{
	for (uint32_t i = 0; i < indent; i++)
		printf("\t");
}

void	print_node(const char *cmd, t_ast_node *node, uint32_t ind)
{
	const char	*type;

	switch (node->type) {
		case COMMAND:
			type = "COMMAND";
			break;
		case LOGIC:
			type = node->logic.type == OR ? "OR" : "AND";
			break;
		case PARENTHESIS:
			type = "SUBSHELL";
			break;
		case PIPELINE:
			type = "PIPELINE";
			break;
	}
	indent(ind);
	printf("%s\n", type);
	switch (node->type) {
		case PARENTHESIS:
			print_node(cmd, node->subsh.contents, ind + 1);
			break;
		case LOGIC:
			printf("L:");
			print_node(cmd, node->logic.l, ind + 1);
			printf("R:");
			print_node(cmd, node->logic.r, ind + 1);
			break;
		case PIPELINE:
			for (size_t i = 0; i < node->pipe.nodes.length; i++)
				print_node(cmd, *((t_ast_node **) dynarr_get(&node->pipe.nodes, i)), ind + 1);
			break;
		case COMMAND:
			indent(ind);
				printf("argv:\n");
			for (t_wordlist *cur = node->command.args.next; cur != NULL; cur = cur->next) {
				indent(ind + 1);
				printf("%s (%s)\n", cur->word, get_flags(cur->flags));
			}
			printf("\n");
			indent(ind);
			printf("redirs:\n");
			for (size_t i = 0; i < node->command.redirs.length; i++) {
				t_redir	*red = dynarr_get(&node->command.redirs, i);
				indent(ind + 1);
				printf("%s - ", g_token_type_names[red->type]);
				if (red->type == RED_HD || red->type == RED_HD_Q) {
					printf("heredoc file: %s\n", red->hd.file);
					printf("heredoc contents:\n");
					for (char **s = red->hd.doc; *s != NULL; ++s) {
						indent(ind + 2);
						printf("%s", *s);
					}
				}
			}
			break;
	}
	indent(ind);
	printf("%s END\n", type);
}
