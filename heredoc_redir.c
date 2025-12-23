/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:38:49 by markhali          #+#    #+#             */
/*   Updated: 2025/12/11 17:40:14 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_dup(int old, int new, const char *msg)
{
	if (dup2(old, new) == -1)
	{
		perror(msg);
		exit(1);
	}
}

void	apply_heredoc(t_node *n)
{
	if (n->heredoc_fd >= 0)
	{
		do_dup(n->heredoc_fd, STDIN_FILENO, "minishell: dup2 (heredoc)");
		close(n->heredoc_fd);
		n->heredoc_fd = -1;
	}
	else if (n->file_fd > 0)
	{
		do_dup(n->file_fd, STDIN_FILENO, "minishell: dup2 (heredoc fallback)");
		close(n->file_fd);
		n->file_fd = -1;
	}
}

void	apply_redir_in(t_node *n)
{
	int	fd;

	fd = open(n->filename, O_RDONLY);
	if (fd < 0)
		return (perror(n->filename), exit(1));
	do_dup(fd, STDIN_FILENO, "minishell: dup2 (input)");
	close(fd);
}

void	apply_redir_out(t_node *n)
{
	int	fd;

	fd = open(n->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(n->filename), exit(1));
	do_dup(fd, STDOUT_FILENO, "minishell: dup2 (output)");
	close(fd);
}

void	apply_redir_append(t_node *n)
{
	int	fd;

	fd = open(n->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror(n->filename), exit(1));
	do_dup(fd, STDOUT_FILENO, "minishell: dup2 (append)");
	close(fd);
}
