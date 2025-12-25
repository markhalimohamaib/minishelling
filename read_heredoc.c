/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:34:22 by markhali          #+#    #+#             */
/*   Updated: 2025/12/24 01:27:33 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_heredoc_warning(const char *delim)
{
	write(2,
		"minishell: warning: here-document delimited by end-of-file (wanted `",
		69);
	write(2, delim, ft_strlen(delim));
	write(2, "')\n", 3);
}

void	write_expanded(t_segment seg, int fd, t_env **env, t_gc *gc)
{
	char	*expanded;

	expanded = check_for_dollar(seg, env, gc);
	if (expanded && expanded[0] != '\0')
		write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
}

void	write_raw(char *line, int fd)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

int	check_heredoc_signal(void)
{
	if (g_signal == SIGINT)
		return (1);
	return (0);
}

int	read_heredoc(const char *delim, int expand, t_env **env, t_gc *gc)
{
	t_heredoc_vars	here;
	int				p[2];
	char			*line;
	int				status;

	if (pipe(p) == -1)
		return (perror("minishell: pipe"), -1);
	here = (t_heredoc_vars){p, delim, expand, env, gc};
	rl_event_hook = check_heredoc_signal;
	while (1)
	{
		line = readline("> ");
		status = handle_heredoc_line(line, &here);
		if (status == -1)
			return (-1);
		if (status == 1)
			return (p[0]);
		process_heredoc_content(line, &here);
		free(line);
	}
}
