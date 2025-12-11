/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:45:15 by markhali          #+#    #+#             */
/*   Updated: 2025/12/11 18:25:02 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_one(char *s, int *i, t_env **env, t_gc *gc)
{
	char	*key;
	char	*val;
	int		j;

	j = 0;
	key = gc_malloc(sizeof(char) * (get_expandable_size_str(s, *i) + 1), gc);
	*i = *i + 1;
	while (s[*i] && is_expandable_char(s[*i]))
		key[j++] = s[(*i)++];
	key[j] = '\0';
	val = replace_val_in_env(key, env, gc);
	return (val);
}

char	*build_expanded(char *s, int *i, t_env **env, t_gc *gc)
{
	char	*leading;
	char	*mid;
	char	*trailing;
	int		start;

	start = *i;
	while (s[*i] && s[*i] != '$')
		(*i)++;
	leading = gc_ft_strndup(s + start, *i - start, gc);
	mid = expand_one(s, i, env, gc);
	start = *i;
	while (s[*i] && s[*i] != '$')
		(*i)++;
	trailing = gc_ft_strndup(s + start, *i - start, gc);
	return (ft_strjoin_plus(leading, mid, trailing, gc));
}

char	*expand_all_dollars(t_segment seg, t_env **env, t_gc *gc)
{
	char	*final;
	char	*tmp;
	int		i;
	int		count;

	i = 0;
	final = gc_ft_strdup("", gc);
	count = count_dollars(seg);
	while (count--)
	{
		tmp = build_expanded(seg.str, &i, env, gc);
		final = gc_ft_strjoin(final, tmp, gc);
	}
	return (final);
}
