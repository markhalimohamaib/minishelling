/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 22:51:16 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/03 23:36:30 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *src, int val, size_t num)
{
	unsigned int	i;

	i = 0;
	while (i < num)
	{
		if (((unsigned char *)src)[i] == (unsigned char)val)
			return ((void *)((unsigned char *)src + i));
		i++;
	}
	return (0);
}
// #include <stdio.h>
// int main (void)
// {
// 	char src[] = "mohamad";
// 	char *d = ft_memchr(src, 'a', 3);
// 	printf("%s", d);
// }