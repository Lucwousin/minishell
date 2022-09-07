/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   destroy_node.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/26 15:11:33 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/26 15:11:33 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <parse.h>
#include <stdlib.h>
#include <redir.h>
#include <libft.h>

static void	destroy_command(t_ast_node *node)
{
	size_t	i;
	t_redir	*red;

	destroy_wordlist(&node->command.args);
	i = 0;
	while (i < node->command.redirs.length)
	{
		red = dynarr_get(&node->command.redirs, i++);
		if (red->type != RED_HD && red->type != RED_HD_Q)
			destroy_wordlist(&red->wl);
		else
		{
			free(red->hd.file);
			ft_free_mult((void **) red->hd.doc);
		}
	}
	dynarr_delete(&node->command.redirs);
}

static void	destroy_logic(t_ast_node *node)
{
	destroy_node(&node->logic.l);
	destroy_node(&node->logic.r);
}

static void	destroy_pipeline(t_ast_node *node)
{
	t_ast_node	**nodes;
	size_t		i;

	nodes = node->pipe.nodes.arr;
	i = node->pipe.nodes.length;
	while (i--)
		destroy_node(nodes + i);
	dynarr_delete(&node->pipe.nodes);
}

static void	destroy_subshell(t_ast_node *node)
{
	destroy_node(&node->subsh.contents);
}

static void	(*g_destroylut[])(t_ast_node *) = {
[COMMAND] = destroy_command,
[LOGIC] = destroy_logic,
[PIPELINE] = destroy_pipeline,
[PARENTHESIS] = destroy_subshell
};

void	destroy_node(t_ast_node **nodep)
{
	if (*nodep == NULL)
		return ;
	g_destroylut[(*nodep)->type](*nodep);
	free(*nodep);
	*nodep = NULL;
}
