/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 20:00:26 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/12 23:04:36 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_for_dollar(t_segment seg, t_env **env, t_gc *gc)
{
	int				count;
	t_val_full_init	val_init;

	val_init.i = 0;
	val_init.j = 0;
	val_init.z = 0;
	if (!(ft_strchr(seg.str, '$')))
		return (seg.str);
	count = count_dollars(seg);
	val_init.expand_val = gc_malloc(sizeof(char)
			* (get_expandable_size(seg) + 1), gc);
	val_init.leading_val = gc_malloc(sizeof(char)
			* (get_lead_val_size(seg) + 1), gc);
	val_init.full_all = gc_ft_strdup("\0", gc);
	while (count)
	{
		initialize(&val_init, &seg);
		initialize2(&val_init, &seg, gc, env);
		count--;
	}
	return (val_init.full_all);
}

int	check_char(t_segment seg, int i)
{
	if (seg.str[i] && seg.str[i] != '\'' && seg.str[i] != ' '
		&& seg.str[i] != '\"' && seg.str[i] != '/' && seg.str[i] != '@'
		&& seg.str[i] != ',' && seg.str[i] != '.' && seg.str[i] != '?'
		&& seg.str[i] != '%' && seg.str[i] != '^' && seg.str[i] != '&'
		&& seg.str[i] != '*' && seg.str[i] != ')' && seg.str[i] != '('
		&& seg.str[i] != '{' && seg.str[i] != '}' && seg.str[i] != '-'
		&& seg.str[i] != '+' && seg.str[i] != '=' && seg.str[i] != ':'
		&& seg.str[i] != ';' && seg.str[i] != '\\' && seg.str[i] != '~'
		&& seg.str[i] != '|' && seg.str[i] != '<' && seg.str[i] != '>'
		&& seg.str[i] != '$' && seg.str[i] != '#')
	{
		return (1);
	}
	return (0);
}

void	initialize(t_val_full_init *val_init, t_segment *seg)
{
	val_init->leading_val[0] = '\0';
	val_init->expand_val[0] = '\0';
	while (seg->str[val_init->i] != '$')
		val_init->leading_val[val_init->j++] = seg->str[val_init->i++];
	val_init->leading_val[val_init->j] = '\0';
	val_init->j = 0;
	if (seg->str[val_init->i] == '$')
		val_init->i++;
	while (check_char(*seg, val_init->i))
		val_init->expand_val[val_init->j++] = seg->str[val_init->i++];
	val_init->expand_val[val_init->j] = '\0';
	val_init->z = val_init->i;
	while (seg->str[val_init->z] && seg->str[val_init->z] != '$')
		val_init->z++;
	val_init->j = 0;
}

void	initialize2(t_val_full_init *val_init, t_segment *seg,
			t_gc *gc, t_env **env)
{
	val_init->trailing_val = gc_ft_strdup("\0", gc);
	val_init->full = gc_ft_strdup("\0", gc);
	val_init->trailing_val = gc_malloc(sizeof(char) * (val_init->z), gc);
	while (seg->str[val_init->i] && seg->str[val_init->z] != '$')
		val_init->trailing_val[val_init->j++] = seg->str[val_init->i++];
	val_init->trailing_val[val_init->j] = '\0';
	val_init->full = ft_strjoin_plus(val_init->leading_val,
			replace_val_in_env(val_init->expand_val, env,
				gc), val_init->trailing_val, gc);
	val_init->full_all = gc_ft_strjoin(val_init->full_all,
			val_init->full, gc);
}
