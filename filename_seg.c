/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_seg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 16:36:20 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/25 22:13:48 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_segment	*filename_seg(t_token *token, t_gc *gc)
{
	int			i;
	int			j;
	char		*org;
	t_segment	*seg;

	i = 0;
	j = 0;
	org = gc_ft_strdup(token->filename, gc);
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

char	*check_for_filename(t_segment seg, t_env **env, t_gc *gc)
{
	int				count;
	t_val_full_init	val_init;

	val_init.i = 0;
	val_init.j = 0;
	val_init.z = 0;
	count = 0;
	if (!seg.str || !(ft_strchr(seg.str, '$')))
		return (seg.str);
	count = count_dollars(seg);
	val_init.full_all = gc_ft_strdup("\0", gc);
	while (count)
	{
		initialize(&val_init, &seg, gc);
		initialize2(&val_init, &seg, gc, env);
		count--;
	}
	return (val_init.full_all);
}

void	handle_expan_file(t_token *token, char *target, t_gc *gc)
{
	t_segment	*file_seg;
	int			j;

	j = 0;
	file_seg = gc_malloc(sizeof(t_segment), gc);
	token->filename = target;
	file_seg = filename_seg(token, gc);
	token->filename = gc_ft_strdup("\0", gc);
	while (file_seg[j].str)
	{
		token->filename = gc_ft_strjoin(token->filename,
				check_for_filename(file_seg[j], gc->envf, gc), gc);
		j++;
	}
}
