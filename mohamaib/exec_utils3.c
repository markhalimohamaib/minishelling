/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:02:33 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/24 00:28:38 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redir_in(t_node *node, t_env **env, t_gc *gc)
{
	int	exit_code;
	
	node->file_fd = open(node->filename, O_RDONLY);
	if (node->file_fd < 0)
	{
		write(2, "minishell: ", 11);
		write(2, node->filename, ft_strlen(node->filename));
		write(2, ": ", 2);
		perror("");
		return (1);
	}
	node->pid1 = fork();
	if (node->pid1 == 0)
	{
		dup2(node->file_fd, STDIN_FILENO);
		close(node->file_fd);
		execute_node(node->left, env, gc);
		exit(0);
	}
	close(node->file_fd);
	waitpid(node->pid1, &node->status, 0);
	exit_code = (node->status >> 8);
	return (exit_code);
}

int	handle_redir_out(t_node *node, t_env **env, t_gc *gc)
{
	int	exit_code;
	
	node->file_fd = open(node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (node->file_fd < 0)
	{
		write(2, "minishell: ", 11);
		write(2, node->filename, ft_strlen(node->filename));
		write(2, ": ", 2);
		perror("");
		return (1);
	}
	node->pid1 = fork();
	if (node->pid1 == 0)
	{
		dup2(node->file_fd, STDOUT_FILENO);
		close(node->file_fd);
		execute_node(node->left, env, gc);
		exit(0);
	}
	close(node->file_fd);
	waitpid(node->pid1, &node->status, 0);
	exit_code = (node->status >> 8);
	return (exit_code);
}

int	handle_redir_append(t_node *node, t_env **env, t_gc *gc)
{
	int	exit_code;
	
	node->file_fd = open(node->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (node->file_fd < 0)
	{
		write(2, "minishell: ", 11);
		write(2, node->filename, ft_strlen(node->filename));
		write(2, ": ", 2);
		perror("");
		return (1);
	}
	node->pid1 = fork();
	if (node->pid1 == 0)
	{
		dup2(node->file_fd, STDOUT_FILENO);
		close(node->file_fd);
		execute_node(node->left, env, gc);
		exit(0);
	}
	close(node->file_fd);
	waitpid(node->pid1, &node->status, 0);
	exit_code = (node->status >> 8);
	return (exit_code);
}

static int	is_delimiter(char *line, char *delimiter)
{
	int	i;

	i = 0;
	while (delimiter[i] && line[i])
	{
		if (delimiter[i] != line[i])
			return (0);
		i++;
	}
	if (delimiter[i] == '\0' && line[i] == '\0')
		return (1);
	return (0);
}

static void	write_heredoc_content(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(2, "minishell: warning: here-document delimited by end-of-file (wanted `", 69);
			write(2, delimiter, ft_strlen(delimiter));
			write(2, "')\n", 3);
			break;
		}
		if (is_delimiter(line, delimiter))
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	handle_redir_heredoc(t_node *node, t_env **env, t_gc *gc)
{
	int		exit_code;

	node->file_fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (node->file_fd < 0)
	{
		perror("minishell: heredoc");
		return (1);
	}
	write_heredoc_content(node->file_fd, node->filename);
	close(node->file_fd);
	node->file_fd = open("/tmp/minishell_heredoc", O_RDONLY);
	if (node->file_fd < 0)
	{
		perror("minishell: heredoc");
		unlink("/tmp/minishell_heredoc");
		return (1);
	}
	node->pid1 = fork();
	if (node->pid1 == 0)
	{
		dup2(node->file_fd, STDIN_FILENO);
		close(node->file_fd);
		execute_node(node->left, env, gc);
		exit(0);
	}
	close(node->file_fd);
	waitpid(node->pid1, &node->status, 0);
	exit_code = (node->status >> 8);
	unlink("/tmp/minishell_heredoc");
	return (exit_code);
}
