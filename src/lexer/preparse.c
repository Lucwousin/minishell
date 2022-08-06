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
#include <libft.h>
#include <stdint.h>
#include <stdlib.h>

#define SUCCESS	0
#define MALLOC	1
#define SYNTAX	2

#define S		0
#define D		1

bool			malloc_error(const char *where);
bool			syntax_error_token(const char *cmd, t_token *token);

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
		return (syntax_error_token(pp->cmd, dynarr_get(pp->tokens, pp->idx)));
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
	if (is_word_type(tok->type) && is_word_type(nxt->type))
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
	if (buf->length == 0
		&& (pp->cur.type < RED_IN || pp->cur.type > RED_APP)
		&& (t->type != DQUOTE && t->type != SQUOTE))
		pp->cur.type = t->type;
	if (t->type == GLOB && pp->glob_count < MAX_GLOBS)
		pp->globs[pp->glob_count++] = buf->length;
	if (!toggle_quote(pp->in_q, t->type))
		if (!expand_tok(pp, buf, t))
			return (MALLOC);
	status = try_concat(t, pp, buf);
	if (status > SUCCESS || \
		(buf->length == 0 && pp->cur.type == VARIABLE && !pp->in_q[D]))
		return (status);
	if (pp->cur.str == NULL && !dynarr_addone(buf, ""))
		return (MALLOC);
	if (pp->cur.str == NULL)
		pp->cur.str = ft_strdup(buf->arr);
	return (status);
}

static t_preparser	init_pp(const char *cmd, t_dynarr *tokens, t_dynarr *etoks)
{
	return ((t_preparser){.cmd = cmd, .tokens = tokens, .output = etoks});
}

bool	preparse(const char *cmd, t_dynarr *tokens, t_dynarr *exp_tokens)
{
	t_preparser	pp;
	uint8_t		err;
	t_dynarr	buf;

	if (!dynarr_create(exp_tokens, tokens->length, sizeof(t_exp_tok)) || \
		!dynarr_create(&buf, 128, sizeof(char)))
		return (err_clean(&pp, MALLOC, exp_tokens));
	pp = init_pp(cmd, tokens, exp_tokens);
	while (pp.idx < tokens->length - 1)
	{
		buf.length = 0;
		pp.cur = (t_exp_tok){END_OF_INPUT, NULL};
		err = expand(&pp, &buf);
		if (err != SUCCESS)
			break ;
		if ((pp.cur.str != NULL && expand_globs(&pp))
			|| (pp.cur.str == NULL && pp.cur.type == VARIABLE))
			continue ;
		err = MALLOC;
		break ;
	}
	dynarr_delete(&buf);
	if (err != SUCCESS || !dynarr_finalize(exp_tokens))
		return (err_clean(&pp, err, exp_tokens));
	return (true);
}
