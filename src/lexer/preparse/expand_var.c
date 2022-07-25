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

#define QUESTION_MARK_STR	"?"
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

static bool	expand_var_words(t_preparser *pp, t_dynarr *buf, char *var_value)
{
	while (*var_value)
	{
		if (*var_value == '*' && pp->glob_count < MAX_GLOBS)
			pp->globs[pp->glob_count++] = buf->length;
		if (get_type(var_value) == WHITE_S || *var_value == '\n')
		{
			if (buf->length != 0 && !add_as_word(pp, buf))
				return (false);
		}
		else if (!dynarr_addone(buf, var_value))
			return (false);
		++var_value;
	}
	return (true);
}

bool	expand_var(t_preparser *pp, t_token *tok, t_dynarr *buf)
{
	char	*var_name;
	char	*var_value;
	bool	is_exit_status;
	bool	retval;

	var_name = ft_substr(pp->cmd, tok->start + 1, tok->end - tok->start);
	if (var_name == NULL)
		return (false);
	is_exit_status = ft_strncmp(var_name, QUESTION_MARK_STR, 2) == 0;
	if (is_exit_status)
		var_value = ft_itoa(pp->exit_status);
	else
		var_value = getenv(var_name);
	free(var_name);
	if (var_value == NULL)
		return (is_exit_status == false);
	if (pp->in_q[D])
		retval = dynarr_add(buf, var_value, ft_strlen(var_value));
	else
		retval = expand_var_words(pp, buf, var_value);
	if (is_exit_status)
		free(var_value);
	return (retval);
}
