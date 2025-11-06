/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 00:31:48 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/06 00:31:50 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*token_type_name(token_type type)
{
	if (type == T_WORD)
		return ("WORD");
	if (type == T_PIPE)
		return ("PIPE");
	if (type == T_REDIR_IN)
		return ("REDIR_IN");
	if (type == T_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == T_REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == T_HEREDOC)
		return ("HEREDOC");
	if (type == T_EOF)
		return ("EOF");
	return ("UNKNOWN");
}

void	print_token_details(t_token *token)
{
	if (token->state)
		printf(" | state=%d", token->state);
	if (token->expand)
		printf(" | expand=%d", token->expand);
	if (token->expand_value && (token->expand_value[0] != '\0'))
		printf(" | expand_value=%s", token->expand_value);
	if (token->filename)
		printf(" | filename=%s", token->filename);
	if (token->heredoc_del)
		printf(" | heredoc_del=%s", token->heredoc_del);
}

void	print_token_list(t_token *head)
{
	t_token	*cur;
	int		i;

	cur = head;
	i = 0;
	printf("==== TOKENS ====\n");
	while (cur)
	{
		printf("[%02d] %-12s | value=\"%s\"", i,
			token_type_name(cur->type), cur->value ? cur->value : "");
		print_token_details(cur);
		printf("\n");
		cur = cur->next;
		i++;
	}
	printf("================\n\n");
}
