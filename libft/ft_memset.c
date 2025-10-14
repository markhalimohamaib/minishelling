/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:25:33 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/04 18:34:34 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t size)
{
	unsigned int	i;
	unsigned char	*ptr;

	ptr = (unsigned char *)str;
	i = 0;
	while (i < size)
	{
		ptr[i] = (unsigned char) c;
		i++;
	}
	return (str);
}
// #include <stdio.h>
// int main (void)
// {
// 	int i = 0;
// 	int a[] = {1,2,4,4,4,5,6,6,5};
// 	while (i < 9)
// 	{
// 		printf("%d", a[i]);
// 		i++;
// 	}
// 	printf("%s", "\n");
// 	ft_memset(a, 1, 3);
// 	i = 0;
// 	while (i < 9)
// 	{
// 		printf("%d", a[i]);
// 		i++;
// 	}
// }