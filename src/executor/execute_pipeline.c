/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_pipeline.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/06 19:31:47 by lsinke        #+#    #+#                 */
/*   Updated: 2022/08/06 19:31:47 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <execute.h>
#include <libft.h>
#include <unistd.h>
#include <stdio.h>
#include <stdnoreturn.h>

#define ERR_PIPEL	"pipe: something went wrong in a pipeline"
#define ERR_PIDS	"pipe: failed to allocate for pids"
#define PIPE_READ	0
#define PIPE_WRITE	1

static bool	init_pipe(t_pipeline *pipe, size_t len)
{
	pipe->idx = 0;
	pipe->pipe[0] = -1;
	pipe->pipe[1] = -1;
	pipe->len = len;
	pipe->pids = malloc(pipe->len * sizeof(pid_t));
	if (pipe->pids == NULL)
		return (perror(ERR_PIDS), EXIT_FAILURE);
	if (dup_stdio(pipe->orig))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

noreturn static
void	run_pipe(t_pipeline *p, t_ast_node *node, int32_t io[2])
{
	if (p->idx != p->len - 1)
		close(p->pipe[PIPE_READ]);
	if (redir_stdio(io))
		exit(execute_node(node, true));
	perror("Redirecting in/output failed!");
	exit(EXIT_FAILURE);
}

static bool	lay_pipe(t_pipeline *p, t_ast_node *node)
{
	const bool	need_pipe = p->idx != p->len - 1;
	int32_t		io[2];

	io[0] = p->pipe[PIPE_READ];
	if (io[0] == -1)
		io[0] = p->orig[STDIN_FILENO];
	if ((need_pipe && (pipe(p->pipe) == -1)))
		return (EXIT_FAILURE);
	io[1] = p->orig[STDOUT_FILENO];
	if (need_pipe)
		io[1] = p->pipe[PIPE_WRITE];
	p->pids[p->idx] = fork();
	if (p->pids[p->idx] != 0 && close(io[0]) | close(io[1]))
		return (EXIT_FAILURE);
	if (p->pids[p->idx] == 0)
		run_pipe(p, node, io);
	return (p->pids[p->idx++] == -1);
}

uint8_t	execute_pipeline(t_ast_node *node, bool must_exit)
{
	t_pipeline	p;
	t_ast_node	**nodes;
	uint8_t		status;

	nodes = node->pipe.nodes.arr;
	if (init_pipe(&p, node->pipe.nodes.length))
		return (EXIT_FAILURE);
	status = EXIT_SUCCESS;
	while (status == EXIT_SUCCESS && p.idx < p.len)
		status = lay_pipe(&p, nodes[p.idx]);
	if (status == EXIT_FAILURE)
		perror(ERR_PIPEL);
	status = wait_pids(p.pids, p.len);
	return (try_exit(status, must_exit));
}
