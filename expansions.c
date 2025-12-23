/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 20:00:26 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/24 00:43:45 by mohamaib         ###   ########.fr       */
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
	count = 0;
	if (!(ft_strchr(seg.str, '$')))
		return (seg.str);
	count = count_dollars(seg);
	val_init.full_all = gc_ft_strdup("\0", gc);
	while (count)
	{
		// init(&val_init);
		initialize(&val_init, &seg, gc);
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

// void	initialize(t_val_full_init *val_init, t_segment *seg, t_gc *gc)
// {
// 	val_init->j = 0;
// 	val_init->expand_val = gc_malloc(sizeof(char)
// 			* (get_expandable_size((*seg)) + 2), gc);
// 	val_init->expand_val[0] = '\0';
// 	val_init->leading_val = gc_malloc(sizeof(char)
// 			* (get_lead_val_size((*seg)) + 2), gc);
// 	val_init->leading_val[0] = '\0';
// 	while (seg->str[val_init->i] != '$')
// 		val_init->leading_val[val_init->j++] = seg->str[val_init->i++];
// 	val_init->leading_val[val_init->j] = '\0';
// 	val_init->j = 0;
// 	if (seg->str[val_init->i] == '$')
// 		val_init->i++;
// 	if (seg->str[val_init->i] == '?')
// 	{
// 		val_init->expand_val[val_init->j++] = '?';
// 		val_init->i++;
// 	}
// 	else if (seg->str[val_init->i] == '$')
// 	{
// 		val_init->expand_val[val_init->j++] = '$';
// 		val_init->i++;
// 	}
// 	else if (!seg->str[val_init->i] || seg->str[val_init->i] == ' ' 
// 		|| seg->str[val_init->i] == '\t' || seg->str[val_init->i] == '$'
// 		|| seg->str[val_init->i] == '\"' || seg->str[val_init->i] == '\''
// 		|| seg->str[val_init->i] == '|' || seg->str[val_init->i] == '<'
// 		|| seg->str[val_init->i] == '>' || !check_char((*seg), val_init->i))
// 	{
//     	val_init->leading_val = gc_ft_strjoin(val_init->leading_val, "$", gc);
//     	val_init->expand_val[0] = '\0';
// 	}
// 	else
// 	{
// 		while (check_char(*seg, val_init->i))
// 			val_init->expand_val[val_init->j++] = seg->str[val_init->i++];
// 	}
// 	val_init->expand_val[val_init->j] = '\0';
// 	val_init->z = val_init->i;
// 	while (seg->str[val_init->z] && seg->str[val_init->z] != '$')
// 		val_init->z++;
// }

void	initialize(t_val_full_init *val_init, t_segment *seg, t_gc *gc)
{
	init_buffers(val_init, seg, gc);
	extract_leading_value(val_init, seg);
	if (seg->str[val_init->i] == '$')
		val_init->i++;
	handle_special_expansion(val_init, seg, gc);
	val_init->expand_val[val_init->j] = '\0';
	find_next_dollar(val_init, seg);
}

void	initialize2(t_val_full_init *val_init, t_segment *seg,
			t_gc *gc, t_env **env)
{
	val_init->j = 0;
	val_init->full = gc_ft_strdup("", gc);
	val_init->trailing_val = gc_malloc(sizeof(char) * ((val_init->z - val_init->i) + 1), gc);
	while (seg->str[val_init->i] && seg->str[val_init->i] != '$')
		val_init->trailing_val[val_init->j++] = seg->str[val_init->i++];
	val_init->trailing_val[val_init->j] = '\0';
	val_init->full = ft_strjoin_plus(val_init->leading_val,
			replace_val_in_env(val_init->expand_val, env,
				gc), val_init->trailing_val, gc);
	val_init->full_all = gc_ft_strjoin(val_init->full_all,
			val_init->full, gc);
}
