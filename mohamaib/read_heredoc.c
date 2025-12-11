/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:34:22 by markhali          #+#    #+#             */
/*   Updated: 2025/12/11 17:34:33 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_heredoc_warning(const char *delim)
{
	write(2,
		"minishell: warning: here-document delimited by end-of-file "
		"(wanted `", 80);
	write(2, delim, ft_strlen(delim));
	write(2, "')\n", 3);
}

static void	write_expanded(t_segment seg, int fd, t_env **env, t_gc *gc)
{
	char	*expanded;

	expanded = check_for_dollar(seg, env, gc);
	if (expanded && expanded[0] != '\0')
		write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
}

static void	write_raw(char *line, int fd)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

int	read_heredoc(const char *delim, int expand, t_env **env, t_gc *gc)
{
	int			p[2];
	char		*line;
	t_segment	seg;

	if (pipe(p) == -1)
		return (perror("minishell: pipe"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (print_heredoc_warning(delim), close(p[1]), p[0]);
		if (ft_strcmp(line, delim) == 0)
			return (free(line), close(p[1]), p[0]);
		if (expand)
		{
			seg.str = line;
			seg.seg_state = NORMAL;
			seg.expands = 1;
			write_expanded(seg, p[1], env, gc);
		}
		else
			write_raw(line, p[1]);
		free(line);
	}
}
