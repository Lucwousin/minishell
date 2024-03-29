/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_input.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/10 14:39:51 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/10 14:39:51 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <input.h>
#include <libft.h>
#include <stdio.h>

// So parse.h does not need to be included
void	destroy_node(t_ast_node **nodep);

void	init_handler(t_in_handler *handler, const char *input)
{
	handler->input = input;
	handler->state = TOKENIZE;
	handler->tokens.arr = NULL;
	handler->root_node = NULL;
	ft_bzero(handler->hooks, DONE * sizeof(t_h_hook));
}

uint8_t	clean_handler(t_in_handler *handler, uint8_t status)
{
	if (handler->tokens.arr != NULL)
		dynarr_delete(&handler->tokens);
	if (handler->root_node != NULL)
		destroy_node(&handler->root_node);
	return (status);
}

uint8_t	handle_input_target(t_in_handler *h, t_hstate target)
{
	uint8_t	status;

	g_env.interrupted = false;
	while (h->state <= target)
	{
		if (h->state == TOKENIZE)
			status = tokenize(&h->tokens, h->input);
		else if (h->state == PARSE)
			status = build_ast(h->input, &h->tokens, &h->root_node);
		else if (h->state == EXECUTE)
			status = execute(h->root_node);
		if (g_env.interrupted || status != SUCCESS)
			break ;
		if (h->hooks[h->state] != NULL)
			h->hooks[h->state](h);
		++h->state;
	}
	return (status);
}

uint8_t	handle_input(const char *input)
{
	t_in_handler	handler;
	uint8_t			status;

	init_handler(&handler, input);
	status = handle_input_target(&handler, EXECUTE);
	if (status != SUCCESS && g_env.exit == 0)
		g_env.exit = status;
	if (g_env.interrupted)
		printf("\n");
	return (clean_handler(&handler, status));
}
