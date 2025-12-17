/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 23:04:03 by markhali          #+#    #+#             */
/*   Updated: 2025/12/17 18:58:43 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_dollars(t_segment seg)
{
	int	i;
	int	doll_count;

	i = 0;
	doll_count = 0;
	while (seg.str[i])
	{
		if (seg.str[i] == '$')
			doll_count++;
		i++;
	}
	return (doll_count);
}

char	*replace_val_in_env(char *val, t_env **env, t_gc *gc)
{
	t_env	*tmp;
	
	if (ft_strcmp(val, "?") == 0)
	{
		char	*exit_status;
		exit_status = get_env(*env, "?");
		if (exit_status)
			return (gc_ft_strdup(exit_status, gc));
		return (gc_ft_strdup("0", gc));
	}
	tmp = (*env);
	while (tmp)
	{
		if (tmp->value && !(ft_strcmp(tmp->key, val)))
		{
			return (gc_ft_strdup(tmp->value, gc));
		}
		tmp = tmp->next;
	}
	return (gc_ft_strdup("", gc));
}

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
