/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:03:04 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/03 23:28:44 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>
#include "libft.h"

void	*ft_calloc(size_t len, size_t varsize)
{
	size_t			i;
	size_t			totsize;
	void			*ptr;

	if (varsize != 0 && len > SIZE_MAX / varsize)
		return (NULL);
	i = 0;
	totsize = len * varsize;
	ptr = (void *)malloc(totsize);
	if (!ptr)
		return (0);
	while (i < totsize)
	{
		((char *)ptr)[i] = 0;
		i++;
	}
	return (ptr);
}
// #include <stdio.h>

// int main(void)
// {
// 	int *arr = (int *)calloc(10, sizeof(long));
// 	if (!arr)
// 		return (1);
// 	for (int i = 0; i < 10; i++)
// 		printf("%d ", arr[i]);
// 	free (arr);
// 	return (0);
// }