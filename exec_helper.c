/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 19:19:25 by markhali          #+#    #+#             */
/*   Updated: 2025/12/19 19:43:27 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*allocate(const char *s, int start, int len, t_gc *gc)
{
	int		i;
	char	*ptr;

	i = 0;
	ptr = gc_malloc(sizeof(char) * (len + 1), gc);
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

char	**gc_ft_split(char const *s, char c, t_gc *gc)
{
	char	**big;
	int		start;
	int		end;
	int		i;
	int		j;

	i = 0;
	j = 0;
	big = gc_malloc(sizeof(char *) * (countwords(s, c) + 1), gc);
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
				big[j++] = allocate(s, start, end - start + 1, gc);
		}
		i++;
	}
	big[j] = NULL;
	return (big);
}

char	*ft_strjoin_plus(char const *s1, char const *s2, char const *s3,
		t_gc *gc)
{
	char	*new;
	int		i;
	int		j;
	int		w;

	if (!s1 && !s2 && !s3)
		return (NULL);
	new = gc_malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)
				+ ft_strlen(s3) + 1), gc);
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	w = 0;
	while (s1 && s1[i] != '\0')
	{
		new[i] = s1[i];
		i++;
	}
	while (s2 && s2[j] != '\0')
		new[i++] = s2[j++];
	while (s3 && s3[w] != '\0')
		new[i++] = s3[w++];
	new[i] = '\0';
	return (new);
}

int	env_count(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->value)
			count++;
		env = env->next;
	}
	return (count);
}
