/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:34:22 by markhali          #+#    #+#             */
/*   Updated: 2025/12/30 20:43:26 by mohamaib         ###   ########.fr       */
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

int	read_heredoc(const char *delim, int expand, t_gc *gc)
{
	int		p[2];
	pid_t	pid;
	int		status;

	status = 0;
	if (pipe(p) == -1)
		return (-1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		heredoc_child(delim, expand, gc, p);
	close(p[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(p[0]);
		g_signal = SIGINT;
		write(1, "\n", 1);
		return (-1);
	}
	return (p[0]);
}
