/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_helper2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:43:02 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/24 00:50:06 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_buffers(t_val_full_init *val_init, t_segment *seg, t_gc *gc)
{
	val_init->j = 0;
	val_init->expand_val = gc_malloc(sizeof(char) * (get_expandable_size((*seg))
				+ 2), gc);
	val_init->expand_val[0] = '\0';
	val_init->leading_val = gc_malloc(sizeof(char) * (get_lead_val_size((*seg))
				+ 2), gc);
	val_init->leading_val[0] = '\0';
}

void	extract_leading_value(t_val_full_init *val_init, t_segment *seg)
{
	while (seg->str[val_init->i] != '$')
		val_init->leading_val[val_init->j++] = seg->str[val_init->i++];
	val_init->leading_val[val_init->j] = '\0';
	val_init->j = 0;
}

int	is_special_char_after_dollar(char c)
{
	if (!c || c == ' ' || c == '\t' || c == '$')
		return (1);
	if (c == '\"' || c == '\'' || c == '|')
		return (1);
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

void	handle_special_expansion(t_val_full_init *val_init, t_segment *seg,
		t_gc *gc)
{
	if (seg->str[val_init->i] == '?')
	{
		val_init->expand_val[val_init->j++] = '?';
		val_init->i++;
	}
	else if (seg->str[val_init->i] == '$')
	{
		val_init->expand_val[val_init->j++] = '$';
		val_init->i++;
	}
	else if (is_special_char_after_dollar(seg->str[val_init->i])
		|| !check_char((*seg), val_init->i))
	{
		val_init->leading_val = gc_ft_strjoin(val_init->leading_val, "$", gc);
		val_init->expand_val[0] = '\0';
	}
	else
	{
		while (check_char(*seg, val_init->i))
			val_init->expand_val[val_init->j++] = seg->str[val_init->i++];
	}
}

void	find_next_dollar(t_val_full_init *val_init, t_segment *seg)
{
	val_init->z = val_init->i;
	while (seg->str[val_init->z] && seg->str[val_init->z] != '$')
		val_init->z++;
}
