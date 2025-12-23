/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:02:33 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/16 18:44:08 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redir_in(t_node *node, t_env **env, t_gc *gc)
{
	int	exit_code;
	int	status;

	node->file_fd = open(node->filename, O_RDONLY);
	if (node->file_fd < 0)
	{
		perror(node->filename);
		return (1);
	}
	node->pid1 = fork();
	if (node->pid1 == 0)
	{
		setup_signals_child();
		dup2(node->file_fd, STDIN_FILENO);
		close(node->file_fd);
		exit(execute_node(node->left, env, gc));
	}
	close(node->file_fd);
	waitpid(node->pid1, &status, 0);
	exit_code = get_exit_code_from_status(status);
	return (exit_code);
}

int	handle_redir_out(t_node *node, t_env **env, t_gc *gc)
{
	int	exit_code;
	int	status;

	node->file_fd = open(node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (node->file_fd < 0)
	{
		perror(node->filename);
		return (1);
	}
	node->pid1 = fork();
	if (node->pid1 == 0)
	{
		setup_signals_child();
		dup2(node->file_fd, STDOUT_FILENO);
		close(node->file_fd);
		exit(execute_node(node->left, env, gc));
	}
	close(node->file_fd);
	waitpid(node->pid1, &status, 0);
	exit_code = get_exit_code_from_status(status);
	return (exit_code);
}

int	handle_redir_append(t_node *node, t_env **env, t_gc *gc)
{
	int	exit_code;
	int	status;

	node->file_fd = open(node->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (node->file_fd < 0)
	{
		perror(node->filename);
		return (1);
	}
	node->pid1 = fork();
	if (node->pid1 == 0)
	{
		setup_signals_child();
		dup2(node->file_fd, STDOUT_FILENO);
		close(node->file_fd);
		exit(execute_node(node->left, env, gc));
	}
	close(node->file_fd);
	waitpid(node->pid1, &status, 0);
	exit_code = get_exit_code_from_status(status);
	return (exit_code);
}

int	handle_redir_heredoc(t_node *node, t_env **env, t_gc *gc)
{
	(void)env;
	(void)gc;
	if (node->heredoc_fd != -1)
	{
		if (dup2(node->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: heredoc dup2");
			return (1);
		}
		close(node->heredoc_fd);
		return (0);
	}
	return (0);
}
