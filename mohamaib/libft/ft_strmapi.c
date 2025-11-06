/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 23:33:53 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/11 19:00:28 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*new;
	unsigned int	size;
	unsigned int	i;

	if (!s || !f)
		return (NULL);
	i = 0;
	size = ft_strlen(s);
	new = (char *)malloc(sizeof (char) * (size + 1));
	while (i < size)
	{
		new[i] = f(i, s[i]);
		i++;
	}
	new[i] = '\0';
	return (new);
}

// char	sample_func(unsigned int i, char c)
// {
// 	if (i % 2 == 0)
// 		return (c - 32);
// 	return (c);
// }

// int	main(void)
// {
// 	char *s = "hello!";
// 	char *result = ft_strmapi(s, sample_func);
// 	printf("%s\n", result); // Output: HeLlO!
// 	free(result);
// 	return (0);
// }