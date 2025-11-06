/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 21:36:22 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/04 01:17:54 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = (t_list *)malloc(sizeof (t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
// #include <stdio.h>

// int main (void)
// {
// 	t_list *node;
// 	// int i;
// 	char *c;

// 	// i = 42;
// 	c = "mohammad";
// 	node = ft_lstnew(c);
// 	printf("%s\n", (char *)node->content);
// 	free (node);
// }