/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:20:14 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/03 23:31:06 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	while (i >= 0)
	{
		if (str[i] == (char)c)
			return ((char *)&str[i]);
		i--;
	}
	return (NULL);
}
// #include <stdio.h>
// int main (void)
// {
// 	char str[] = "mohammad tee shir\t";
// 	char s[] = "tripouille";
// 	int i = 't';
// 	printf("%s", ft_strrchr(str, i));
// 	printf("%s", ft_strrchr(s, 't' + 256));
// }