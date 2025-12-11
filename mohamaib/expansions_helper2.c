/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_helper2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 18:24:44 by markhali          #+#    #+#             */
/*   Updated: 2025/12/11 18:36:16 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_expandable_char(char c)
{
	return (c && c != ' ' && c != '\'' && c != '"' && c != '/' && c != '@'
		&& c != ',' && c != '.' && c != '?' && c != '%' && c != '^'
		&& c != '&' && c != '*' && c != ')' && c != '(' && c != '{'
		&& c != '}' && c != '-' && c != '+' && c != '=' && c != ':'
		&& c != ';' && c != '\\' && c != '~' && c != '|' && c != '<'
		&& c != '>' && c != '$' && c != '#');
}

int	get_expandable_size_str(char *s, int i)
{
	int	len;

	len = 0;
	while (s[i] && is_expandable_char(s[i]))
	{
		len++;
		i++;
	}
	return (len);
}

char	*gc_ft_strndup(char *s, int n, t_gc *gc)
{
	char	*out;
	int		i;

	out = gc_malloc(n + 1, gc);
	i = 0;
	while (i < n)
	{
		out[i] = s[i];
		i++;
	}
	out[i] = '\0';
	return (out);
}
