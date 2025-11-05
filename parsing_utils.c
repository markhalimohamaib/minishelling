/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 23:33:01 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/05 23:38:56 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_split(char const *s, char c)
{
	char	**big;
	int		start;
	int		end;
	int		i;
	int		j;

	i = 0;
	j = 0;
	big = (char **)malloc(sizeof(char *) * (countwords(s, c) + 1));
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
