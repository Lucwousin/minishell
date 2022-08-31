#ifndef TEST_H
# define TEST_H
# include <ms_types.h>

void	perform_test(void);
void	print_tokens(const char *cmd, t_token *start, t_token *end);
void	print_node(const char *cmd, t_ast_node *node, uint32_t ind);

#endif //TEST_H
