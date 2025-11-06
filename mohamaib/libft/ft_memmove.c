/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 21:39:24 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/04 19:07:15 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t size)
{
	int	i;

	if (!src || !dest)
		return (0);
	if (((unsigned char *)dest) > ((const unsigned char *)src))
	{
		i = size - 1;
		while (i >= 0)
		{
			((unsigned char *)dest)[i] = ((const unsigned char *)src)[i];
			i--;
		}
		return (dest);
	}
	if (((unsigned char *)dest) < ((const unsigned char *)src))
	{
		i = 0;
		while ((unsigned int)i < size)
		{
			((unsigned char *)dest)[i] = ((const unsigned char *)src)[i];
			i++;
		}
		return (dest);
	}
	return (dest);
}
// #include <stdio.h>
// int main (void)
// {
// 	char src[] = "123456789";
// 	printf("%s\n", src);
// 	ft_memmove(src , src + 5, 4);
// 	printf("%s", src);
// 		return (0);
// }