/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 01:25:21 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/04 00:59:44 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

int	countwords(const char *s, char c)
{
	int	i;
	int	count;
	int	isword;

	if (!s)
		return (0);
	i = 0;
	count = 0;
	while (s[i])
	{
		isword = 0;
		if ((i == 0 && s[0] != c) || (s[i] != c && s[i - 1] == c))
			isword = 1;
		if (isword == 1)
			count++;
		i++;
	}
	return (count);
}

char	*allocate(const char *s, int start, int len)
{
	int		i;
	char	*ptr;

	i = 0;
	ptr = (char *)malloc(sizeof (char) * (len + 1));
	if (!ptr)
		return (NULL);
	while (s[start] && i < len)
	{
		ptr[i] = s[start + i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	**ft_split(char const *s, char c)
{
	char	**big;
	int		start;
	int		end;
	int		i;
	int		j;

	i = 0;
	j = 0;
	big = (char **)malloc(sizeof (char *) * (countwords(s, c) + 1));
	if (!big)
		return (NULL);
	while (s[i])
	{
		if ((i == 0 && s[0] != c) || (s[i] != c && s[i - 1] == c))
			start = i;
		if ((s[i] != c && s[i + 1] == c) || (s[i] != c && s[i + 1] == '\0'))
		{
			end = i;
			if (j < countwords(s, c))
				big[j++] = allocate(s, start, end - start + 1);
		}
		i++;
	}
	big[j] = NULL;
	return (big);
}
// #include <stdio.h>
// int	main(void)
// {
// 	char	**result;
// 	int		i = 0;
// 	char	*str = "hello world this is C";
// 	char	delimiter = ' ';
// 	result = ft_split(str, delimiter);
// 	if (!result)
// 	{
// 		printf("ft_split returned NULL\n");
// 		return (1);
// 	}
// 	while (result[i])
// 	{
// 		printf("result[%d]: %s\n", i, result[i]);
// 		free(result[i]);
// 		i++;
// 	}
// 	free(result);
// 	return (0);
// }