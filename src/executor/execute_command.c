/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_command.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/27 14:44:02 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/27 14:44:02 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <execute.h>
#include <builtins.h>
#include <redir.h>
#include <signals.h>
#include <libft.h>
#include <unistd.h>
#include <stdio.h>

static bool	do_redirs(t_dynarr *redirs, int32_t orig[2], bool builtin)
{
	size_t	i;
	int32_t	fds[2];

	if (builtin && !dup_stdio(orig))
		return (false);
	fds[0] = -1;
	fds[1] = -1;
	i = 0;
	while (i < redirs->length)
	{
		if (redirect(((t_redir *) redirs->arr) + i++, fds) || !builtin)
			continue ;
		if (fds[0] != -1)
			close(fds[0]);
		if (fds[1] != -1)
			close(fds[0]);
		return (false);
	}
	if (redir_stdio(fds) || !builtin)
		return (true);
	redir_stdio(orig);
	return (false);
}

static bool	argv_to_arr(char ***dst, t_wordlist *cur, t_dynarr *buf)
{
	static const void	*null = NULL;

	dynarr_delete(buf);
	if (!dynarr_create(buf, 16, sizeof(char *)))
		return (false);
	while (cur->next)
	{
		cur = cur->next;
		if (!dynarr_addone(buf, &cur->word))
			return (false);
	}
	if (!dynarr_addone(buf, &null) || \
		!dynarr_finalize(buf))
		return (false);
	*dst = buf->arr;
	buf->arr = NULL;
	return (true);
}

static uint8_t	expand(char ***dst, t_wordlist *head)
{
	t_dynarr	buf;

	if (!dynarr_create(&buf, 128, sizeof(char)))
	{
		perror("expand_command");
		return (ERROR);
	}
	if (expand_variables(head, &buf)
		&& split_words(head)
		&& remove_quotes(head, &buf)
		&& expand_filenames(head)
		&& argv_to_arr(dst, head, &buf))
		return (SUCCESS);
	dynarr_delete(&buf);
	return (ERROR);
}

static uint8_t	cleanup_exit(uint8_t status, bool must_exit, char **argv)
{
	free(argv);
	return (try_exit(status, must_exit));
}

uint8_t	execute_command(t_ast_node *node, bool must_exit)
{
	char		**argv;
	t_builtin	builtin;
	uint8_t		status;
	int32_t		orig[2];

	if (expand(&argv, &node->command.args) != SUCCESS)
		try_exit(ERROR, must_exit);
	builtin = identify_command(argv);
	if (builtin == NONE && !must_exit)
	{
		if (fork_and_wait(&status))
			return (cleanup_exit(status, must_exit, argv));
		must_exit = true;
	}
	if (must_exit && reset_signals() != SUCCESS)
		cleanup_exit(EXIT_FAILURE, must_exit, argv);
	if (node->command.redirs.length > 0)
		if (!do_redirs(&node->command.redirs, orig, builtin != NONE))
			return (cleanup_exit(EXIT_FAILURE, must_exit, argv));
	if (builtin == NONE)
		execute_binary(argv);
	status = execute_builtin(builtin, argv);
	if (node->command.redirs.length > 0)
		redir_stdio(orig);
	return (cleanup_exit(status, must_exit, argv));
}
