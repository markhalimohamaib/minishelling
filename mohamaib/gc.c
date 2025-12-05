/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:29:28 by markhali          #+#    #+#             */
/*   Updated: 2025/12/02 15:18:02 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	gc_init(t_gc *gc)
{
	gc->list = NULL;
}

void	*gc_malloc(size_t size, t_gc *gc)
{
	void		*ptr;
	t_gc_node	*node;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	node = malloc(sizeof(t_gc_node));
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	node->ptr = ptr;
	node->next = gc->list;
	gc->list = node;
	return (ptr);
}

void	gc_free_all(t_gc *gc)
{
	t_gc_node	*cur;
	t_gc_node	*next;

	cur = gc->list;
	while (cur)
	{
		next = cur->next;
		free(cur->ptr);
		free(cur);
		cur = next;
	}
	gc->list = NULL;
}

char	*gc_ft_strdup(const char *str, t_gc *gc)
{
	char	*dup;
	int		i;

	i = 0;
	dup = gc_malloc((sizeof (char) * (ft_strlen(str) + 1)), gc);
	if (!dup)
		return (0);
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*gc_ft_strjoin(char const *s1, char const *s2, t_gc *gc)
{
	char	*new;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	new = gc_malloc(sizeof (char) * (ft_strlen(s1) + ft_strlen(s2) + 1), gc);
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while (s1 && s1[i] != '\0')
	{
		new[i] = s1[i];
		i++;
	}
	while (s2 && s2[j] != '\0')
		new[i++] = s2[j++];
	new[i] = '\0';
	return (new);
}
