/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_count.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 19:09:11 by markhali          #+#    #+#             */
/*   Updated: 2025/12/11 19:12:17 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_single_quotes(const char *str, int *i)
{
	(*i)++;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (str[*i] == '\'')
		(*i)++;
	return (1);
}

static int	skip_double_quotes(const char *str, int *i)
{
	(*i)++;
	while (str[*i] && str[*i] != '"')
		(*i)++;
	if (str[*i] == '"')
		(*i)++;
	return (1);
}

static int	skip_normal(const char *str, int *i)
{
	while (str[*i] && str[*i] != '\'' && str[*i] != '"'
		&& str[*i] != '|' && str[*i] != '>' && str[*i] != '<')
		(*i)++;
	return (1);
}

int	count_segments(const char *org)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (org[i])
	{
		if (org[i] == '\'')
			count += skip_single_quotes(org, &i);
		else if (org[i] == '"')
			count += skip_double_quotes(org, &i);
		else
			count += skip_normal(org, &i);
	}
	return (count);
}
