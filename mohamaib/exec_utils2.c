/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 20:24:40 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/10 20:08:08 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

int	execute_pipe(t_node *node, t_env **env, t_gc *gc)
{
	int	exit_code;

	pipe(node->pipefd);
	node->pid1 = fork();
	if (node->pid1 == 0)
	{
		dup2(node->pipefd[1], STDOUT_FILENO);
		close_fds(node->pipefd);
		execute_node(node->left, env, gc);
		exit(0);
	}
	node->pid2 = fork();
	if (node->pid2 == 0)
	{
		dup2(node->pipefd[0], STDIN_FILENO);
		close_fds(node->pipefd);
		execute_node(node->right, env, gc);
		exit(0);
	}
	close_fds(node->pipefd);
	waitpid(node->pid1, NULL, 0);
	waitpid(node->pid2, &node->status, 0);
	exit_code = (node->status >> 8);
	return (exit_code);
}

int	execute_redir(t_node *node, t_env **env, t_gc *gc)
{
	int	exit_code;

	if (node->redir_type == T_REDIR_IN)
		return (handle_redir_in(node, env, gc));
	else if (node->redir_type == T_REDIR_OUT)
		return (handle_redir_out(node, env, gc));
	else if (node->redir_type == T_REDIR_APPEND)
		return (handle_redir_append(node, env, gc));
	else if (node->redir_type == T_HEREDOC)
	{
		exit_code = handle_redir_heredoc(node, env, gc);
		dup2(node->file_fd, STDIN_FILENO);
		close(node->file_fd);
		return (exit_code);
	}
	return (1);
}
