/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 22:16:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/25 17:00:28 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_single_quoted(char *origin, int *i, t_gc *gc)
{
	int		w;
	int		j;
	int		size;
	char	*str;

	(*i)++;
	w = 0;
	size = 0;
	j = (*i);
	while (origin[j] && origin[j] != '\'')
	{
		j++;
		size++;
	}
	str = gc_malloc(sizeof(char) * (size + 1), gc);
	while (origin[(*i)] && origin[(*i)] != '\'')
	{
		str[w] = origin[(*i)];
		(*i)++;
		w++;
	}
	str[w] = '\0';
	(*i)++;
	return (str);
}

char	*read_double_quoted(char *origin, int *i, t_gc *gc)
{
	int		w;
	int		j;
	int		size;
	char	*str;

	(*i)++;
	w = 0;
	size = 0;
	j = (*i);
	while (origin[j] && origin[j] != '\"')
	{
		j++;
		size++;
	}
	str = gc_malloc(sizeof(char) * (size + 1), gc);
	while (origin[(*i)] && origin[(*i)] != '\"')
	{
		str[w] = origin[(*i)];
		(*i)++;
		w++;
	}
	str[w] = '\0';
	(*i)++;
	return (str);
}

char	*read_unquoted(char *origin, int *i, t_gc *gc)
{
	int		w;
	int		j;
	int		size;
	char	*str;

	w = 0;
	j = (*i);
	size = 0;
	while (origin[j] && origin[j] != '\'' && origin[j] != '\"'
		&& origin[j] != '|' && origin[j] != '>' && origin[j] != '<')
	{
		j++;
		size++;
	}
	str = gc_malloc(sizeof(char) * (size + 1), gc);
	while (origin[(*i)] && origin[(*i)] != '\'' && origin[(*i)] != '\"'
		&& origin[(*i)] != '|' && origin[(*i)] != '>' && origin[(*i)] != '<')
	{
		str[w] = origin[(*i)];
		(*i)++;
		w++;
	}
	str[w] = '\0';
	return (str);
}

void	set_segment(t_segment *seg, char *str, t_token_state state,
		int expands)
{
	seg->str = str;
	seg->seg_state = state;
	seg->expands = expands;
}

t_segment	*build_segment(t_token *token, t_gc *gc)
{
	int			i;
	int			j;
	char		*org;
	t_segment	*seg;

	i = 0;
	j = 0;
	org = gc_ft_strdup(token->origin_val, gc);
	token->seg_count = count_segments(org);
	seg = gc_malloc(sizeof(t_segment) * (token->seg_count + 1), gc);
	while (org[i])
	{
		if (org[i] == '\'')
			set_segment(&seg[j], read_single_quoted(org, &i, gc), IN_SINGLE, 0);
		else if (org[i] == '"')
			set_segment(&seg[j], read_double_quoted(org, &i, gc), IN_DOUBLE, 1);
		else
			set_segment(&seg[j], read_unquoted(org, &i, gc), NORMAL, 1);
		j++;
	}
	seg[j].str = NULL;
	seg[j].seg_state = NORMAL;
	seg[j].expands = 0;
	return (seg);
}
