/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:58:25 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/04 18:56:52 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t size)
{
	size_t					i;
	const unsigned char		*s;
	unsigned char			*d;

	s = (const unsigned char *)src;
	d = (unsigned char *)dest;
	i = 0;
	while (i < size)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

// #include <stdio.h>
// int main (void)
// {
// 	char src[] = "mohamad";
// 	char dest[10];
// 	ft_memcpy(dest, src, 5);
// 	printf("%s", dest);
// 	printf("%s", "\n");
// 	int i[] = {1,2,3,4,5,6};
// 	int desti[8];
// 	ft_memcpy(desti, i, 6 * sizeof(int));
// 	int j = 0;
// 	while (j < 6)
// 	{
// 		printf("%d", desti[j]);
// 		j++;
// 	}
// }