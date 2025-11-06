/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_build.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/05 23:59:59 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*find_last_redir(t_token *start)
{
	t_token	*tmp;
	t_token	*last_redir;

	tmp = start;
	last_redir = NULL;
	while (tmp && tmp->type != T_PIPE)
	{
		if (tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT
			|| tmp->type == T_REDIR_APPEND || tmp->type == T_HEREDOC)
			last_redir = tmp;
		tmp = tmp->next;
	}
	return (last_redir);
}

t_node	*wrap_with_redir(t_token *redir, t_node *node, t_gc *gc)
{
	if (redir->filename)
		return (create_redir_node(redir->type, redir->filename, node, gc));
	else if (redir->heredoc_del)
		return (create_redir_node(redir->type, redir->heredoc_del, node, gc));
	else
		return (create_redir_node(redir->type, "", node, gc));
}

void	skip_to_pipe(t_token **head)
{
	while (*head && (*head)->type != T_PIPE)
		*head = (*head)->next;
}

t_node	*parse_simple_cmd(t_token **head, t_gc *gc)
{
	t_node	*node;
	t_token	*tmp;
	t_token	*last_redir;

	tmp = (*head);
	node = create_cmd_node(build_cmd_array(head, gc), gc);
	last_redir = find_last_redir(tmp);
	while (last_redir)
	{
		node = wrap_with_redir(last_redir, node, gc);
		last_redir->type = T_EOF;
		last_redir = find_last_redir(tmp);
	}
	skip_to_pipe(head);
	return (node);
}

t_node	*parse_pipeline(t_token **head, t_gc *gc)
{
	t_node	*left;
	t_node	*right;

	if (!(*head) || (*head)->type == T_PIPE)
		return (NULL);
	left = parse_simple_cmd(head, gc);
	if ((*head) && (*head)->type == T_PIPE)
	{
		(*head) = (*head)->next;
		right = parse_pipeline(head, gc);
		return (create_pipe_node(left, right, gc));
	}
	return (left);
}
