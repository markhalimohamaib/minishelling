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

static char	*read_line_from_stdin(void)
{
	char	*buffer;
	char	*new_buffer;
	char	c;
	int		size;
	int		capacity;
	int		bytes_read;

	buffer = malloc(1024);
	if (!buffer)
		return (NULL);
	capacity = 1024;
	size = 0;
	while (1)
	{
		bytes_read = read(STDIN_FILENO, &c, 1);
		if (bytes_read <= 0)
		{
			if (size == 0)
			{
				free(buffer);
				return (NULL);
			}
			buffer[size] = '\0';
			return (buffer);
		}
		if (c == '\n')
		{
			buffer[size] = '\0';
			return (buffer);
		}
		if (size + 1 >= capacity)
		{
			capacity *= 2;
			new_buffer = malloc(capacity);
			if (!new_buffer)
			{
				free(buffer);
				return (NULL);
			}
			ft_memcpy(new_buffer, buffer, size);
			free(buffer);
			buffer = new_buffer;
		}
		buffer[size++] = c;
	}
}

int	read_heredoc(const char *delim, int expand, t_env **env, t_gc *gc)
{
	t_heredoc_vars		here;
	int					p[2];
	char				*line;
	int					status;
	struct sigaction	sa_int;
	struct sigaction	sa_quit;
	struct sigaction	old_int;
	struct sigaction	old_quit;

	if (pipe(p) == -1)
		return (perror("minishell: pipe"), -1);
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = handle_heredoc_sigint;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGINT, &sa_int, &old_int);
	sigaction(SIGQUIT, &sa_quit, &old_quit);
	g_signal = 0;
	here = (t_heredoc_vars){p, delim, expand, env, gc};
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = read_line_from_stdin();
		status = handle_heredoc_line(line, &here);
		if (status == -1)
		{
			sigaction(SIGINT, &old_int, NULL);
			sigaction(SIGQUIT, &old_quit, NULL);
			return (-1);
		}
		if (status == 1)
		{
			sigaction(SIGINT, &old_int, NULL);
			sigaction(SIGQUIT, &old_quit, NULL);
			return (p[0]);
		}
		process_heredoc_content(line, &here);
		free(line);
	}
}
