/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/22 20:49:50 by mohamaib         ###   ########.fr       */
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
	return (node);
}

t_node	*create_redir_node(token_type type, char *filename,
						t_node *left, t_gc *gc)
{
	t_node	*node;

	node = gc_malloc(sizeof(t_node), gc);
	node->type = REDIR_NODE;
	node->cmd = NULL;
	node->filename = gc_ft_strdup(filename, gc);
	node->redir_type = type;
	node->left = left;
	node->right = NULL;
	node->builtin = BLT_NONE;
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

char	**build_cmd_array(t_token **head, t_gc *gc)
{
	int		i;
	char	**cmd;
	t_token	*tmp;

	i = 0;
	tmp = (*head);
	cmd = gc_malloc((sizeof(char *) * (count_cmd_words(*head) + 1)), gc);
	while (tmp && tmp->type != T_PIPE)
	{
		if (tmp->type == T_WORD)
			cmd[i++] = gc_ft_strdup(tmp->value, gc);
		tmp = tmp->next;
	}
	cmd[i] = NULL;
	return (cmd);
}
