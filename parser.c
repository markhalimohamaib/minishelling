/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/11 20:04:27 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*create_cmd_node(char **cmd, t_gc *gc)
{
	t_node	*node;

	node = gc_malloc(sizeof(t_node), gc);
	node->type = CMD_NODE;
	node->cmd = cmd;
	node->filename = NULL;
	node->redir_type = 0;
	node->right = NULL;
	node->left = NULL;
	node->builtin = BLT_NONE;
	node->heredoc_fd = -1;
	node->heredoc_expand = 0;
	node->file_fd = -1;
	return (node);
}

t_node	*create_redir_node(t_token_type type, t_token *red_tok, t_node *left,
		t_gc *gc)
{
	t_node	*node;

	node = gc_malloc(sizeof(t_node), gc);
	node->type = REDIR_NODE;
	node->cmd = NULL;
	node->heredoc_expand = red_tok->herdoc_expand;
	if (red_tok->heredoc_del)
		node->filename = gc_ft_strdup(red_tok->heredoc_del, gc);
	else if (red_tok->filename)
		node->filename = gc_ft_strdup(red_tok->filename, gc);
	else
		node->filename = NULL;
	node->redir_type = type;
	node->left = left;
	node->right = NULL;
	node->builtin = BLT_NONE;
	node->heredoc_fd = -1;
	node->file_fd = -1;
	return (node);
}

t_node	*create_pipe_node(t_node *left, t_node *right, t_gc *gc)
{
	t_node	*node;

	node = gc_malloc(sizeof(t_node), gc);
	node->type = PIPE_NODE;
	node->left = left;
	node->right = right;
	node->cmd = NULL;
	node->filename = NULL;
	node->redir_type = 0;
	node->builtin = BLT_NONE;
	node->heredoc_fd = -1;
	node->file_fd = -1;
	node->heredoc_expand = 0;
	return (node);
}

int	count_cmd_words(t_token *head)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = head;
	while (tmp && tmp->type != T_PIPE)
	{
		if (tmp->type == T_WORD)
			count++;
		tmp = tmp->next;
	}
	return (count);
}
