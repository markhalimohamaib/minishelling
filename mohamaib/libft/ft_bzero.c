/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:32:13 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/04 18:46:43 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *str, size_t num)
{
	unsigned int	i;

	i = 0;
	while (i < num)
	{
		((unsigned char *)str)[i] = '\0';
		i++;
	}
}
// #include <stdio.h>
// int main (void)
// {
// 	char str[] = "mohammad ibrahim";
// 	printf("%s", str);
// 	ft_bzero(str, 5);
// 	printf("%s", str);
// 	printf("%s", "\n");
// 	int a[] = {1,2,3,4,5,6};
// 	int i = 0;
// 	while (i < 6)
// 	{
// 		printf("%d", a[i]);
// 		i++;
// 	}
// 	printf("%s", "\n");
// 	ft_bzero(a, 16);
// 	i = 0;
// 	while (i < 6)
// 	{
// 		printf("%d", a[i]);
// 		i++;
// 	}
// }