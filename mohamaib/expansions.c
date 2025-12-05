/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 20:00:26 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/05 23:06:55 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_val_in_env(char *val, t_env **env)
{
	t_env	*tmp;

	// char	*env_val;
	tmp = (*env);
	while (tmp)
	{
		if (!(ft_strcmp(tmp->key, val)))
		{
			return (ft_strdup(tmp->value));
			// env_val = gc_malloc(sizeof(char) * (ft_strlen(tmp->value) + 1),
					// gc);
			// while()
		}
		tmp = tmp->next;
	}
	return (NULL);
}

// char	*fill_leading_val(t_segment seg)
// {
// 	int		i;
// 	char	*lead;

// 	i = 0;
// 	while(seg.str[i] != '$')
// 	{

// 	}
// }

int	get_lead_val_size(t_segment seg)
{
	int	size;
	int	i;

	i = 0;
	size = 0;
	while (seg.str[i] != '$')
	{
		i++;
		size++;
	}
	return (size);
}

int	get_expandable_size(t_segment seg)
{
	int	size;
	int	i;

	i = 0;
	size = 0;
	while (seg.str[i])
	{
		if (seg.str[i] == '$')
		{
			i++;
			while (seg.str[i] && seg.str[i] != ' ' && seg.str[i] != '|'
				&& seg.str[i] != '>' && seg.str[i] != '<')
			{
				size++;
				i++;
			}
			break ;
		}
		i++;
	}
	return (size);
}

char	*check_for_dollar(t_segment seg, t_env **env, t_gc *gc)
{
	int		i;
	int		j;
	char	*expand_val;
	char	*leading_val;
	char	*full;

	i = 0;
	j = 0;
	if (!(ft_strchr(seg.str, '$')))
		return (seg.str);
	expand_val = gc_malloc(sizeof(char) * (get_expandable_size(seg) + 1), gc);
	leading_val = gc_malloc(sizeof(char) * (get_lead_val_size(seg) + 1), gc);
	// leading_val = fill_leading_val(seg);
	while (seg.str[i] != '$')
	{
		leading_val[j] = seg.str[i];
		i++;
		j++;
	}
	leading_val[j] = '\0';
	j = 0;
	i++;
	while (seg.str[i])
	{
		expand_val[j] = seg.str[i];
		i++;
		j++;
	}
	expand_val[j] = '\0';
	full = gc_ft_strjoin(leading_val, replace_val_in_env(expand_val, env), gc);
	return (full);
	// seg.str = full;
}
