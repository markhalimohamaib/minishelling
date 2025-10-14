/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 22:12:47 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/03 23:31:34 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

// size_t	ft_strlen(const char *s)
// {
// 	size_t	i;

// 	i = 0;
// 	while (s[i])
// 		i++;
// 	return (i);
// }

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new;
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	if (start >= ft_strlen(s))
	{
		new = (char *)malloc(1);
		if (!new)
			return (0);
		new[0] = '\0';
		return (new);
	}
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	new = (char *)malloc(len + 1);
	if (!new)
		return (0);
	while (s[start] != '\0' && i < len)
		new[i++] = s[start++];
	new[i] = '\0';
	return (new);
}
// #include <stdio.h>
// int main (void)
// {
// 	char *s = "42 Network";
// 	char *sub = ft_substr(s, 3, 7); // asking for more than available
// 	printf("Result: \"%s\"\n", sub); // Output: "Network"
// 	free(sub);
// }