/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 20:00:26 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/08 02:51:58 by mohamaib         ###   ########.fr       */
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

	// char	*env_val;
	tmp = (*env);
	while (tmp->next)
	{
		if (!(ft_strcmp(tmp->key, val)))
		{
			return (gc_ft_strdup(tmp->value, gc));
			// env_val = gc_malloc(sizeof(char) * (ft_strlen(tmp->value) + 1),
			// gc);
			// while()
		}
		tmp = tmp->next;
	}
	return (gc_ft_strdup("\0", gc));
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

// int	get_trail_val_size(t_segment seg)
// {
// 	int	i;
// 	int	size;

// 	i = 0;
// 	size = 0;

// }

char	*check_for_dollar(t_segment seg, t_env **env, t_gc *gc)
{
	int		i;
	int		j;
	int		z;
	int		count;
	char	*expand_val;
	char	*leading_val;
	char	*trailing_val;
	char	*full;
	char	*full_all;

	i = 0;
	j = 0;
	z = 0;
	if (!(ft_strchr(seg.str, '$')))
		return (seg.str);
	count = count_dollars(seg);
	expand_val = gc_malloc(sizeof(char) * (get_expandable_size(seg) + 1), gc);
	leading_val = gc_malloc(sizeof(char) * (get_lead_val_size(seg) + 1), gc);
	// leading_val = fill_leading_val(seg);
	full_all = gc_ft_strdup("\0", gc);
	while (count)
	{
		leading_val[0] = '\0';
		expand_val[0] = '\0';
		trailing_val = gc_ft_strdup("\0", gc);
		full = gc_ft_strdup("\0", gc);
		while (seg.str[i] != '$')
			leading_val[j++] = seg.str[i++];
		leading_val[j] = '\0';
		j = 0;
		if (seg.str[i] == '$')
			i++;
		while (seg.str[i] && seg.str[i] != '\'' && seg.str[i] != ' '
			&& seg.str[i] != '/' && seg.str[i] != '@' && seg.str[i] != ','
			&& seg.str[i] != '.' && seg.str[i] != '?' && seg.str[i] != '#'
			&& seg.str[i] != '^' && seg.str[i] != '&' && seg.str[i] != '*'
			&& seg.str[i] != ')' && seg.str[i] != '(' && seg.str[i] != '{'
			&& seg.str[i] != '}' && seg.str[i] != '-' && seg.str[i] != '+'
			&& seg.str[i] != '=' && seg.str[i] != ':' && seg.str[i] != ';'
			&& seg.str[i] != '\\' && seg.str[i] != '~' && seg.str[i] != '|'
			&& seg.str[i] != '<' && seg.str[i] != '>' && seg.str[i] != '$')
			expand_val[j++] = seg.str[i++];
		expand_val[j] = '\0';
		z = i;
		while (seg.str[z] && seg.str[z] != '$')
			z++;
		j = 0;
		trailing_val = gc_malloc(sizeof(char) * (z), gc);
		while (seg.str[i] && seg.str[z] != '$')
			trailing_val[j++] = seg.str[i++];
		trailing_val[j] = '\0';
		full = ft_strjoin_plus(leading_val, replace_val_in_env(expand_val, env,
					gc), trailing_val, gc);
		full_all = gc_ft_strjoin(full_all, full, gc);
		count--;
	}
	return (full_all);
	// seg.str = full;
}
