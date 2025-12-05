/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 20:00:26 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/02 00:10:34 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_val_in_env(t_segment seg, char *val, t_gc *gc)
{
	
}

int	get_expandable_size(t_segment seg)
{
	int	size;
	int	i;

	i = 0;
	size = 0;
	while(seg.str[i])
	{
		if (seg.str[i] == '$')
		{
			i++;
			while(seg.str[i] && seg.str[i] != ' ' && seg.str[i] != '|' 
					&& seg.str[i] != '>' && seg.str[i] != '<')
			{
				size++;
				i++;
			}
			break;
		}
		i++;
	}
	return (size);
}

void	check_for_dollar(t_segment seg, t_gc *gc)
{
	int		i;
	int		j;
	int		size;
	char	*expand_val;

	i = 0;
	size = 0;
	j = 0;
	expand_val = gc_malloc(sizeof(char) * (size + 1), gc);
	while(seg.str[i])
	{
		if (seg.str[i] == '$')
		{
			i++;
			while(seg.str[i] && seg.str[i] != ' ' && seg.str[i] != '|' 
					&& seg.str[i] != '>' && seg.str[i] != '<')
			{
				expand_val[j] = seg.str[i];
				i++;
				j++;
			}
			check_val_in_env(seg, expand_val, gc);
			break;
		}
		i++;
	}
}
