/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/05 23:59:59 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	word_len(char *str, int i)
{
	int		count;
	char	quote;

	count = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i] && str[i] != ' ' && str[i] != '\t'
		&& str[i] != '|' && str[i] != '>' && str[i] != '<')
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i++];
			count++;
			while (str[i] && str[i] != quote)
			{
				count++;
				i++;
			}
			if (str[i] == quote)
			{
				count++;
				i++;
			}
		}
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

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

t_token	*create_token(token_type type, char *value, t_gc *gc)
{
	t_token	*new;

	new = gc_malloc(sizeof(t_token), gc);
	new->type = type;
	new->value = value;
	new->state = NORMAL;
	new->expand = 0;
	new->expand_value = NULL;
	new->filename = NULL;
	new->heredoc_del = NULL;
	new->next = NULL;
	return (new);
}

void	handle_operator(char *str, t_token **head, int *i, t_gc *gc)
{
	t_token	*token;

	token = NULL;
	if (str[(*i)] == '|')
		token = create_token(T_PIPE, "|", gc);
	else if (str[(*i)] == '>' && str[(*i) + 1] == '>')
	{
		token = create_token(T_REDIR_APPEND, ">>", gc);
		(*i)++;
	}
	else if (str[(*i)] == '>')
		token = create_token(T_REDIR_OUT, ">", gc);
	else if (str[(*i)] == '<' && str[(*i) + 1] == '<')
	{
		token = create_token(T_HEREDOC, "<<", gc);
		(*i)++;
	}
	else if (str[(*i)] == '<')
		token = create_token(T_REDIR_IN, "<", gc);
	(*i)++;
	add_token_to_list(head, token);
	if (token && token->type != T_PIPE)
		process_redir_target(token, str, i, gc);
}

t_token	*tokenize_input(char *str, t_gc *gc)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (str[i])
	{
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		if (!str[i])
			break ;
		if (str[i] == '|' || str[i] == '>' || str[i] == '<')
			handle_operator(str, &head, &i, gc);
		else if (str[i] == '\'' || str[i] == '\"')
			handle_quoted_word(str, &head, &i, gc);
		else
			handle_regular_word(str, &head, &i, gc);
	}
	return (head);
}
