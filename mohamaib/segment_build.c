/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 22:16:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/02 16:53:35 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_segments(const char *org)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (org[i])
	{
		if (org[i] == '\'')
		{
			count++;
			i++;
			while (org[i] && org[i] != '\'')
				i++;
			if (org[i] == '\'')
				i++;
		}
		else if (org[i] == '"')
		{
			count++;
			i++;
			while (org[i] && org[i] != '"')
				i++;
			if (org[i] == '"')
				i++;
		}
		else
		{
			count++;
			while (org[i] && org[i] != '\'' && org[i] != '"' && org[i] != '|'
				&& org[i] != '>' && org[i] != '<')
				i++;
		}
	}
	return (count);
}

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
		{
			seg[j].str = read_single_quoted(org, &i, gc);
			seg[j].seg_state = IN_SINGLE;
			seg[j].expands = 0;
		}
		else if (org[i] == '\"')
		{
			seg[j].str = read_double_quoted(org, &i, gc);
			seg[j].seg_state = IN_DOUBLE;
			seg[j].expands = 1;
		}
		else
		{
			seg[j].str = read_unquoted(org, &i, gc);
			seg[j].seg_state = NORMAL;
			seg[j].expands = 1;
		}
		j++;
	}
	seg[j].str = NULL;
	seg[j].expands = NORMAL;
	seg[j].expands = 0;
	return (seg);
}
