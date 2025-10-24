/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:29:28 by markhali          #+#    #+#             */
/*   Updated: 2025/10/24 17:14:05 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	gc_init(t_gc *gc)
{
	gc->list = NULL;
}

void	*gc_malloc(t_gc *gc, size_t size)
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
