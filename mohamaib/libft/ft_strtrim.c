/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:09:24 by mohamaib          #+#    #+#             */
/*   Updated: 2025/06/07 01:54:50 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

// int	ft_strlen(char const *s1)
// {
// 	int	i;

// 	i = 0;
// 	while (s1[i] != '\0')
// 		i++;
// 	return (i);
// }

int	get_start(char const *s1, char const *set, int setsz)
{
	int	i;
	int	j;

	i = 0;
	while (s1[i])
	{
		j = 0;
		while (set[j])
		{
			if (s1[i] == set[j])
				break ;
			j++;
		}
		if (j == setsz)
			return (i);
		i++;
	}
	return (i);
}

int	get_end(char const *s1, char const *set, int setsz)
{
	int	i;
	int	j;

	i = (int)(ft_strlen(s1) - 1);
	while (i >= 0)
	{
		j = 0;
		while (set[j])
		{
			if (s1[i] == set[j])
				break ;
			j++;
		}
		if (j == setsz)
			return (i);
		i--;
	}
	return (-1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		start;
	int		end;
	char	*trimd;

	if (!s1 || !set)
		return (NULL);
	start = get_start(s1, set, ft_strlen(set));
	end = get_end(s1, set, ft_strlen(set));
	if (end < start)
		return (ft_strdup(""));
	trimd = (char *)malloc(sizeof (char) * ((end - start) + 2));
	if (!trimd)
		return (NULL);
	i = start;
	j = 0;
	while (i <= end)
	{
		trimd[j] = s1[i];
		i++;
		j++;
	}
	trimd[j] = '\0';
	return (trimd);
}
// #include <stdio.h>
// int main (void)
// {
// 	char s1[] = ".     / .";
// 	char set[] = ".";
// 	char *trimd = ft_strtrim(s1, set);
// 	printf("result is \"%s\"", trimd);
// 	free (trimd);
// }