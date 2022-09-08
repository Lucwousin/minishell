/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/20 12:58:22 by lsinke        #+#    #+#                 */
/*   Updated: 2022/04/20 12:58:22 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <get_next_line.h>
#include <unistd.h>

static t_list	*find_or_create_list(t_list **list, int fd)
{
	t_list		*cur;
	t_content	*content;

	cur = *list;
	while (cur)
	{
		content = cur->content;
		if (content->fd == fd)
			return (cur);
		cur = cur->next;
	}
	content = malloc(sizeof(t_content));
	if (!content)
		return (NULL);
	content->leftover = NULL;
	content->fd = fd;
	cur = ft_lstnew(content);
	if (!cur)
		free(content);
	else
		ft_lstadd_front(list, cur);
	return (cur);
}

/**
 * Checks if leftover contains a newline. If not, read BUFFER_SIZE bytes
 * and append them onto leftover. Repeat.
 *
 * Returns NULL if anything went wrong
 */
static char	*read_until_newline(int fd, char *leftover)
{
	char	*buffer;
	char	*tmp;
	ssize_t	read_bytes;

	if (leftover && ft_strchr(leftover, '\n'))
		return (leftover);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	buffer[0] = '\0';
	while (!ft_strchr(buffer, '\n'))
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes <= 0)
			break ;
		buffer[read_bytes] = '\0';
		tmp = leftover;
		leftover = ft_strjoin(tmp, buffer);
		free(tmp);
		if (!leftover)
			break ;
	}
	free(buffer);
	return (leftover);
}

/**
 * Return everything in leftover up to (and including) the first newline
 * If there is no newline, returns leftover and sets leftover_p to NULL
 * If there is a newline, allocate a new string and copy everything.
 */
static char	*get_line(char **leftover_p, char *leftover)
{
	char	*line;
	char	*newline_idx;
	size_t	len;

	newline_idx = ft_strchr(leftover, '\n');
	if (!newline_idx)
	{
		line = leftover;
		*leftover_p = NULL;
		return (line);
	}
	len = newline_idx - leftover + 1 + 1;
	line = malloc(len * sizeof(char));
	if (!line)
	{
		free(leftover);
		*leftover_p = NULL;
		return (NULL);
	}
	ft_memcpy(line, leftover, len - 1);
	line[len - 1] = '\0';
	return (line);
}

/**
 * Get everything after the first '\n' and put it in a new string.
 * Always frees leftover
 * If the newline was the last character in the string, return NULL
 */
static char	*get_leftover(char *leftover)
{
	char	*str;
	char	*newline_idx;
	size_t	len;
	size_t	n;

	len = ft_strlen(leftover);
	newline_idx = ft_strchr(leftover, '\n');
	n = len - (newline_idx - leftover);
	if (n != 1)
		str = malloc(n * sizeof(char));
	else
		str = NULL;
	if (str)
		ft_memcpy(str, newline_idx + 1, n);
	free(leftover);
	return (str);
}

char	*get_next_line(int fd)
{
	static t_list	*list = NULL;
	t_list			*cur;
	t_content		*content;
	char			*line;

	if (fd < 0)
		return (NULL);
	cur = find_or_create_list(&list, fd);
	if (!cur)
		return (NULL);
	content = cur->content;
	content->leftover = read_until_newline(fd, content->leftover);
	if (content->leftover != NULL)
		line = get_line(&content->leftover, content->leftover);
	else
		line = NULL;
	if (content->leftover != NULL)
		content->leftover = get_leftover(content->leftover);
	if (content->leftover == NULL)
		ft_lstdelelem(&list, cur, &free);
	return (line);
}
