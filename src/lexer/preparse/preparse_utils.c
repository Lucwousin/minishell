/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   preparse_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/25 16:14:15 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/25 16:14:15 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>

static bool	should_expand_var(t_preparser *pp, t_token *t)
{
	t_tokentype	type;

	type = t->type;
	return (type == VARIABLE
		&& !pp->in_q[0]
		&& pp->cur.type != RED_HD_Q
		&& pp->cur.type != RED_HD);
}

static bool	should_add_str(t_preparser *pp, t_token *t)
{
	t_tokentype	type;

	type = t->type;
	return (type == WORD || type == GLOB
		|| pp->in_q[0] || pp->in_q[1]);
}

bool	expand_tok(t_preparser *pp, t_dynarr *buf, t_token *t)
{
	if (should_expand_var(pp, t))
		return (expand_var(pp, t, buf));
	else if (should_add_str(pp, t))
		return (add_token(pp->cmd, buf, t));
	return (true);
}
