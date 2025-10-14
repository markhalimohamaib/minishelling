/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 00:08:57 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/04 19:28:50 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((const unsigned char *)s1)[i] != ((const unsigned char *)s2)[i])
			return (((const unsigned char *)s1)[i] - \
				((const unsigned char *)s2)[i]);
		i++;
	}
	return (0);
}
// #include <stdio.h>
// int main (void)
// {
// 	char s1[] = "mhmd";
// 	char s2[] = "mhma";
// 	printf("%d", ft_memcmp(s1, s2, 4));
// }
