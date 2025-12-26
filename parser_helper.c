/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:22:57 by markhali          #+#    #+#             */
/*   Updated: 2025/12/26 23:21:03 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_segments(t_token *tmp, char **cmd_i, t_env **env, t_gc *gc)
{
	int	j;

	j = 0;
	while (tmp->segment[j].str)
	{
		if (tmp->segment[j].expands == 1)
			tmp->segment[j].str = gc_ft_strdup(
					check_for_dollar(tmp->segment[j], env, gc), gc);
		*cmd_i = gc_ft_strjoin(*cmd_i, tmp->segment[j].str, gc);
		j++;
	}
}

static void	fill_cmd_words(char **cmd, t_token **head, t_env **env, t_gc *gc)
{
	t_token	*tmp;
	int		i;

	tmp = *head;
	i = 0;
	while (tmp && tmp->type != T_PIPE)
	{
		if (tmp->type == T_WORD)
		{
			cmd[i] = gc_ft_strdup("", gc);
			append_segments(tmp, &cmd[i], env, gc);
			if (cmd[i][0] != '\0')
				i++;
		}
		tmp = tmp->next;
	}
	cmd[i] = NULL;
}

char	**build_cmd_array(t_token **head, t_env **env, t_gc *gc)
{
	char	**cmd;
	int		count;

	count = count_cmd_words(*head);
	cmd = gc_malloc(sizeof(char *) * (count + 1), gc);
	fill_cmd_words(cmd, head, env, gc);
	return (cmd);
}
