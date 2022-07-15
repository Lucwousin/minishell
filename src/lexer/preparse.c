/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   preparse.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/15 14:12:54 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/15 14:12:54 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>
#include <stdint.h>
#include <stdlib.h>
#include "libft.h"

#define SUCCESS		0
#define NO_TOKEN	1
#define MALLOC		2
#define SYNTAX		3

#define SQUOTE_I	0
#define DQUOTE_I	1

bool			malloc_error(const char *where);
bool			syntax_error(const char *cmd, t_token *token);

static bool	err_clean(t_preparser *pp, uint8_t status, t_dynarr *ex_toks)
{
	t_exp_tok	*arr;

	arr = ex_toks->arr;
	while (ex_toks->length--)
		free(arr[ex_toks->length].str);
	dynarr_delete(ex_toks);
	if (status == MALLOC)
		return (malloc_error("preparser"));
	else
		return (syntax_error(pp->cmd, dynarr_get(pp->tokens, pp->idx)));
}

static bool	expand_var(t_preparser *pp, t_token *tok, t_dynarr *buf)
{
	char	*var_name;
	char	*var_value;

	// TODO: $?
	var_name = ft_substr(pp->cmd, tok->start + 1, tok->end - tok->start);
	if (var_name == NULL)
		return (false);
	var_value = getenv(var_name);
	free(var_name);
	return (var_value == NULL \
		|| dynarr_add(buf, var_value, ft_strlen(var_value)));
}

static uint8_t	expand(t_preparser *pp, t_dynarr *buf);

static uint8_t	try_concat(t_token *tok, t_preparser *pp, t_dynarr *buf)
{
	t_token	*nxt;

	nxt = dynarr_get(pp->tokens, pp->idx);
	if (pp->in_q[SQUOTE_I])
		if (nxt->type != SQUOTE)
			return (expand(pp, buf));
	if (pp->in_q[DQUOTE_I])
		if (nxt->type != DQUOTE)
			return (expand(pp, buf));
	if ((tok->type == VARIABLE && nxt->type == WORD) || \
		(nxt->type == VARIABLE && tok->type == WORD))
		if (tok->end == nxt->start - 1)
			return (expand(pp, buf));
	if (tok->type != OPERATOR)
		return (SUCCESS);
	id_operator(pp->cmd, tok, nxt);
	if (tok->type == OR || tok->type == AND || tok->type == PIPE)
		return (SUCCESS);
	if (nxt->type == SQUOTE || nxt->type == DQUOTE || nxt->type == WORD)
		return (expand(pp, buf));
	return (SYNTAX);
}

static uint8_t	expand(t_preparser *pp, t_dynarr *buf)
{
	t_token	*t;
	uint8_t	status;

	t = dynarr_get(pp->tokens, pp->idx++);
	if (toggle_quote(pp->in_q, t->type))
		return (NO_TOKEN);
	if (t->type == VARIABLE && !pp->in_q[SQUOTE_I])
	{
		if (!expand_var(pp, t, buf))
			return (MALLOC);
	}
	else if ((t->type == WORD || pp->in_q[SQUOTE_I] || pp->in_q[DQUOTE_I]) \
			&& !add_token(pp->cmd, buf, t))
		return (MALLOC);
	status = try_concat(t, pp, buf);
	if (status == SYNTAX || status == MALLOC)
		return (status);
	if (!dynarr_addone(buf, ""))
		return (MALLOC);
	pp->cur = (t_exp_tok){t->type, ft_strdup(buf->arr)};
	return (status);
}

bool	preparse(t_dynarr *tokens, const char *cmd, t_dynarr *exp_tokens)
{
	t_preparser	pp;
	uint8_t		err;
	t_dynarr	buf;

	pp = ((t_preparser){cmd, tokens, 0, {}, {false, false}});
	if (!dynarr_create(exp_tokens, tokens->length, sizeof(t_exp_tok)) || \
		!dynarr_create(&buf, 128, sizeof(char)))
		return (err_clean(&pp, MALLOC, exp_tokens));
	while (pp.idx < tokens->length - 1)
	{
		buf.length = 0;
		err = expand(&pp, &buf);
		if (err == NO_TOKEN)
			continue ;
		if (err != SUCCESS)
			break ;
		if (pp.cur.str != NULL && dynarr_addone(exp_tokens, &pp.cur))
			continue ;
		err = MALLOC;
		break ;
	}
	dynarr_delete(&buf);
	if ((err != SUCCESS && err != NO_TOKEN) || !dynarr_finalize(exp_tokens))
		return (err_clean(&pp, err, exp_tokens));
	return (true);
}
