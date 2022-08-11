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

#include <input.h>
#include <token.h>
#include <libft.h>

#define SUCCESS	0
#define ERROR	1
#define SYNTAX	2

#define S		0
#define D		1

bool			general_error(const char *where);
void			syntax_error(const char *where);

bool	syntax_error_token(const char *cmd, t_token *token)
{
	char	*substr;

	substr = ft_substr(cmd, token->start, token->end - token->start + 1);
	if (substr == NULL)
		return (general_error("syntax_error"));
	syntax_error(substr);
	free(substr);
	return (false);
}

static uint8_t	err_clean(t_preparser *pp, t_dynarr *buf, uint8_t status)
{
	t_exp_tok	*arr;

	if (buf->arr != NULL)
		dynarr_delete(buf);
	arr = pp->output->arr;
	while (pp->output->length--)
		free(arr[pp->output->length].str);
	dynarr_delete(pp->output);
	if (status == ERROR)
		general_error("preparser");
	else
		syntax_error_token(pp->cmd, dynarr_get(pp->tokens, pp->idx));
	dynarr_delete(pp->tokens);
	return (status);
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
			return (ERROR);
	status = try_concat(t, pp, buf);
	if (status > SUCCESS || \
		(buf->length == 0 && pp->cur.type == VARIABLE && !pp->in_q[D]))
		return (status);
	if (pp->cur.str == NULL && !dynarr_addone(buf, ""))
		return (ERROR);
	if (pp->cur.str == NULL)
		pp->cur.str = ft_strdup(buf->arr);
	return (status);
}

uint8_t	preparse(const char *cmd, t_dynarr *tokens, t_dynarr *exp_tokens)
{
	t_preparser	pp;
	uint8_t		status;
	t_dynarr	buf;

	if (!dynarr_create(exp_tokens, tokens->length, sizeof(t_exp_tok)) || \
		!dynarr_create(&buf, 128, sizeof(char)))
		return (err_clean(&pp, &buf, ERROR));
	ft_bzero(&pp, sizeof(t_preparser));
	pp.cmd = cmd;
	pp.tokens = tokens;
	pp.output = exp_tokens;
	while (pp.idx < tokens->length - 1)
	{
		buf.length = 0;
		pp.cur = (t_exp_tok){END_OF_INPUT, NULL};
		status = expand(&pp, &buf);
		if (status != SUCCESS)
			return (err_clean(&pp, &buf, status));
		if ((pp.cur.str != NULL && !expand_globs(&pp)) || \
			(pp.cur.str == NULL && pp.cur.type != VARIABLE))
			return (err_clean(&pp, &buf, ERROR));
	}
	dynarr_delete(&buf);
	dynarr_delete(tokens);
	return (status);
}
