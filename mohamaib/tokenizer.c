/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/11 18:59:39 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token_to_list(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!(*head))
	{
		(*head) = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_token	*create_token(t_token_type type, char *value, t_gc *gc)
{
	t_token	*new;

	new = gc_malloc(sizeof(t_token), gc);
	new->type = type;
	new->value = value;
	new->state = NORMAL;
	new->expand_line = 0;
	new->seg_count = 0;
	new->origin_val = NULL;
	new->segment = NULL;
	new->filename = NULL;
	new->heredoc_del = NULL;
	new->herdoc_expand = 0;
	new->next = NULL;
	return (new);
}
