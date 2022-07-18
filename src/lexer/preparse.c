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

#define SUCCESS	0
#define MALLOC	1
#define SYNTAX	2

#define S		0
#define D		1

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
	if (var_value == NULL)
		return (true);
	if (pp->in_q[D])
		return (dynarr_add(buf, var_value, ft_strlen(var_value)));
	while (*var_value)
	{
		if (get_type(var_value) != WHITE_S && *var_value != '\n')
		{
			if (!dynarr_addone(buf, var_value))
				return (false);
		}
		else if (buf->length != 0)
		{
			if (!dynarr_addone(buf, ""))
				return (false);
			pp->cur = (t_exp_tok){WORD, ft_strdup(buf->arr)};
			if (pp->cur.str == NULL || !dynarr_addone(pp->output, &pp->cur))
				return (false);
			pp->cur.str = NULL;
			buf->length = 0;
		}
		++var_value;
	}
	return (true);
}

static uint8_t	expand(t_preparser *pp, t_dynarr *buf);

static uint8_t	try_concat(t_token *tok, t_preparser *pp, t_dynarr *buf)
{
	t_token	*nxt;

	if (pp->in_q[S] || pp->in_q[D])
	{
		if (pp->cur.type == RED_HD)
			pp->cur.type = RED_HD_Q;
		return (expand(pp, buf));
	}
	nxt = dynarr_get(pp->tokens, pp->idx);
	if (tok->type >= WORD && tok->type <= DQUOTE && \
		nxt->type >= WORD && nxt->type <= DQUOTE)
		if (tok->end == nxt->start - 1)
			return (expand(pp, buf));
	if (tok->type != OPERATOR)
		return (SUCCESS);
	id_operator(pp->cmd, tok);
	pp->cur.type = tok->type;
	if (tok->type < RED_IN || tok->type > RED_APP)
		return (SUCCESS);
	if (nxt->type < WORD || nxt->type > DQUOTE)
		return (SYNTAX);
	return (expand(pp, buf));
}

static uint8_t	expand(t_preparser *pp, t_dynarr *buf)
{
	t_token	*t;
	uint8_t	status;

	t = dynarr_get(pp->tokens, pp->idx++);
	if (buf->length == 0 && (pp->cur.type < RED_IN || pp->cur.type > RED_APP))
		pp->cur.type = t->type;
	if (t->type == VARIABLE && !pp->in_q[S] && \
		pp->cur.type != RED_HD_Q && pp->cur.type != RED_HD)
	{
		if (!expand_var(pp, t, buf))
			return (MALLOC);
	}
	else if (!toggle_quote(pp->in_q, t->type)
		&& (t->type == WORD || pp->in_q[0] || pp->in_q[1]))
		if (!add_token(pp->cmd, buf, t))
			return (MALLOC);
	status = try_concat(t, pp, buf);
	if (status == SYNTAX || status == MALLOC)
		return (status);
	if (pp->cur.str == NULL && !dynarr_addone(buf, ""))
		return (MALLOC);
	if (pp->cur.str == NULL)
		pp->cur.str = ft_strdup(buf->arr);
	return (status);
}

bool	preparse(t_dynarr *tokens, const char *cmd, t_dynarr *exp_tokens)
{
	t_preparser	pp;
	uint8_t		err;
	t_dynarr	buf;

	pp = ((t_preparser){cmd, tokens, exp_tokens, 0, {}, {false, false}});
	if (!dynarr_create(exp_tokens, tokens->length, sizeof(t_exp_tok)) || \
		!dynarr_create(&buf, 128, sizeof(char)))
		return (err_clean(&pp, MALLOC, exp_tokens));
	while (pp.idx < tokens->length - 1)
	{
		buf.length = 0;
		pp.cur.str = NULL;
		err = expand(&pp, &buf);
		if (err != SUCCESS)
			break ;
		if (pp.cur.str != NULL && dynarr_addone(exp_tokens, &pp.cur))
			continue ;
		err = MALLOC;
		break ;
	}
	dynarr_delete(&buf);
	if (err != SUCCESS || !dynarr_finalize(exp_tokens))
		return (err_clean(&pp, err, exp_tokens));
	return (true);
}
