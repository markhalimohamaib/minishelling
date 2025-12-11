/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:51:13 by markhali          #+#    #+#             */
/*   Updated: 2025/12/11 01:33:11 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_heredoc(const char *delim, int expand, t_env **env, t_gc *gc)
{
	int			p[2];
	char		*line;
	char		*expanded;
	t_segment	seg;

	if (pipe(p) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			/* EOF before delimiter
				-> print warning like bash and stop reading */
			write(2,
				"minishell: warning: here-document delimited by end-of-file (wanted `",
				69);
			write(2, delim, ft_strlen(delim));
			write(2, "')\n", 3);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		if (expand)
		{
			seg.str = line;
			seg.seg_state = NORMAL;
			seg.expands = 1;
			expanded = check_for_dollar(seg, env, gc); /* GC-allocated */
			if (expanded && expanded[0] != '\0')
				write(p[1], expanded, ft_strlen(expanded));
			write(p[1], "\n", 1);
			free(line);
			/* expanded is GC-managed — do not free here */
		}
		else
		{
			write(p[1], line, ft_strlen(line));
			write(p[1], "\n", 1);
			free(line);
		}
	}
	/* close writer end and return the reader */
	close(p[1]);
	return (p[0]);
}

void	prepare_heredocs(t_node *n, t_env **env, t_gc *gc)
{
	if (!n)
		return ;
	/* process current node first (pre-order) so we read heredocs in the same
		order they appear in the command line tokens */
	if (n->type == REDIR_NODE && n->redir_type == T_HEREDOC)
	{
		/* node->filename holds the delimiter (parser already stripped quotes) */
		n->heredoc_fd = read_heredoc(n->filename, n->heredoc_expand, env, gc);
		/* if read_heredoc failed it returns -1; we keep that sentinel */
	}
	prepare_heredocs(n->left, env, gc);
	prepare_heredocs(n->right, env, gc);
}

void	apply_redirs(t_node *n, t_env **env, t_gc *gc)
{
	int	fd;

	if (!n)
		return ;
	fd = 0;
	/* first apply the right subtree */
	apply_redirs(n->right, env, gc);
	if (n->type == REDIR_NODE)
	{
		if (n->redir_type == T_HEREDOC)
		{
			if (n->heredoc_fd >= 0)
			{
				if (dup2(n->heredoc_fd, STDIN_FILENO) == -1)
				{
					perror("minishell: dup2 (heredoc)");
					exit(1);
				}
				close(n->heredoc_fd);
				n->heredoc_fd = -1;
			}
			else
			{
				/* fallback: if no prepared fd,
					try opening a file_fd if present */
				if (n->file_fd > 0)
				{
					if (dup2(n->file_fd, STDIN_FILENO) == -1)
					{
						perror("minishell: dup2 (heredoc fallback)");
						exit(1);
					}
					close(n->file_fd);
					n->file_fd = -1;
				}
			}
		}
		else if (n->redir_type == T_REDIR_IN)
		{
			fd = open(n->filename, O_RDONLY);
			if (fd < 0)
			{
				perror(n->filename);
				exit(1);
			}
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("minishell: dup2 (input)");
				close(fd);
				exit(1);
			}
			close(fd);
		}
		else if (n->redir_type == T_REDIR_OUT)
		{
			fd = open(n->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror(n->filename);
				exit(1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("minishell: dup2 (output)");
				close(fd);
				exit(1);
			}
			close(fd);
		}
		else if (n->redir_type == T_REDIR_APPEND)
		{
			fd = open(n->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				perror(n->filename);
				exit(1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("minishell: dup2 (append)");
				close(fd);
				exit(1);
			}
			close(fd);
		}
	}
	/* then apply left subtree */
	apply_redirs(n->left, env, gc);
}

void	cleanup_heredocs(t_node *node)
{
	if (!node)
		return ;
	cleanup_heredocs(node->left);
	cleanup_heredocs(node->right);
	if (node->type == REDIR_NODE)
	{
		if (node->redir_type == T_HEREDOC && node->heredoc_fd >= 0)
		{
			close(node->heredoc_fd);
			node->heredoc_fd = -1;
		}
	}
}
