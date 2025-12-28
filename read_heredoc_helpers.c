/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 01:12:57 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/24 01:21:29 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc_line(char *line, t_heredoc_vars *vars)
{
	if (g_signal == SIGINT)
	{
		if (line)
			free(line);
		close(vars->p[1]);
		close(vars->p[0]);
		return (-1);
	}
	if (!line)
	{
		print_heredoc_warning(vars->delim);
		close(vars->p[1]);
		return (1);
	}
	if (ft_strcmp(line, vars->delim) == 0)
	{
		free(line);
		close(vars->p[1]);
		return (1);
	}
	return (0);
}

void	process_heredoc_content(char *line, t_heredoc_vars *here)
{
	t_segment	seg;

	if (here->expand)
	{
		seg.str = line;
		seg.seg_state = NORMAL;
		seg.expands = 1;
		write_expanded(seg, here->p[1], here->env, here->gc);
	}
	else
		write_raw(line, here->p[1]);
}
