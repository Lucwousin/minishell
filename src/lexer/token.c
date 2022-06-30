/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/30 18:19:28 by lsinke        #+#    #+#                 */
/*   Updated: 2022/06/30 18:19:28 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <token.h>
#include <libft.h>

void	resize_tokens(t_tokens *data, size_t new_len)
{
	t_codepoint	*new_allocated;

	new_allocated = ft_realloc(
			data->tokens,
			data->tokens_size * sizeof(t_codepoint),
			new_len * sizeof(t_codepoint));
	if (new_allocated == NULL)
		// TODO: This should probably display an error msg
		// Note that realloc does not free the pointer if an error occured
		exit(EXIT_FAILURE);
	data->tokens = new_allocated;
	data->tokens_size = new_len;
}
