/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:51:13 by markhali          #+#    #+#             */
/*   Updated: 2025/12/16 19:38:53 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prepare_heredocs(t_node *n, t_env **env, t_gc *gc)
{
	if (!n)
		return (0);
	if (n->type == REDIR_NODE && n->redir_type == T_HEREDOC)
	{
		n->heredoc_fd = read_heredoc(n->filename, n->heredoc_expand, env, gc);
		if (n->heredoc_fd == -1)
			return (-1);
	}
	if (prepare_heredocs(n->left, env, gc) == -1)
		return (-1);
	if (prepare_heredocs(n->right, env, gc) == -1)
		return (-1);
	return (0);
}

void	apply_redirs(t_node *n, t_env **env, t_gc *gc)
{
	(void)env;
	(void)gc;
	if (!n)
		return ;
	apply_redirs(n->right, env, gc);
	if (n->type == REDIR_NODE)
	{
		if (n->redir_type == T_HEREDOC)
			apply_heredoc(n);
		else if (n->redir_type == T_REDIR_IN)
			apply_redir_in(n);
		else if (n->redir_type == T_REDIR_OUT)
			apply_redir_out(n);
		else if (n->redir_type == T_REDIR_APPEND)
			apply_redir_append(n);
	}
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
