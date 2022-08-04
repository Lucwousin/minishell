/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_var.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/18 16:36:57 by lsinke        #+#    #+#                 */
/*   Updated: 2022/07/18 16:36:57 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>
#include <libft.h>
#include <environ.h>

#define S					0
#define D					1

static bool	add_as_word(t_preparser *pp, t_dynarr *buf)
{
	if (!dynarr_addone(buf, ""))
		return (false);
	pp->cur = (t_exp_tok){VARIABLE, ft_strdup(buf->arr)};
	if (pp->cur.str == NULL || !expand_globs(pp))
		return (false);
	pp->cur.str = NULL;
	buf->length = 0;
	return (true);
}

static bool	expand_var_words(t_preparser *pp, t_dynarr *buf, const char *var)
{
	while (*var)
	{
		if (*var == '*' && pp->glob_count < MAX_GLOBS)
			pp->globs[pp->glob_count++] = buf->length;
		if (get_type(var) == WHITE_S || *var == '\n')
		{
			if (buf->length != 0 && !add_as_word(pp, buf))
				return (false);
		}
		else if (!dynarr_addone(buf, var))
			return (false);
		++var;
	}
	return (true);
}

bool	expand_var(t_preparser *pp, t_token *tok, t_dynarr *buf)
{
	char		*var_name;
	const char	*var_value;
	bool		retval;

	var_name = ft_substr(pp->cmd, tok->start + 1, tok->end - tok->start);
	if (var_name == NULL)
		return (false);
	var_value = get_variable(var_name);
	free(var_name);
	if (var_value == NULL)
		return (true);
	if (pp->in_q[D])
		retval = dynarr_add(buf, var_value, ft_strlen(var_value));
	else
		retval = expand_var_words(pp, buf, var_value);
	return (retval);
}
