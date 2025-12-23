/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 18:58:15 by markhali          #+#    #+#             */
/*   Updated: 2025/12/23 20:19:20 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	word_len_in_quotes(char *str, int *i)
{
	int		count;
	char	quote;

	count = 0;
	quote = str[*i];
	(*i)++;
	count++;
	while (str[*i] && str[*i] != quote)
	{
		count++;
		(*i)++;
	}
	if (str[*i] == quote)
	{
		count++;
		(*i)++;
	}
	return (count);
}

int	word_len(char *str, int i)
{
	int	count;

	count = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i] && str[i] != ' ' && str[i] != '\t'
		&& str[i] != '|' && str[i] != '>' && str[i] != '<')
	{
		if (str[i] == '\'' || str[i] == '\"')
			count += word_len_in_quotes(str, &i);
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

void	process_redir_target(t_token *token, char *str, int *i, t_gc *gc)
{
	int		size;
	char	*target;

	token->herdoc_expand = 1;
	size = word_len(str, (*i));
	target = extract_word(str, i, size, gc);
	if ((target[0] == '\'' && target[ft_strlen(target) - 1] == '\'')
		|| (target[0] == '"' && target[ft_strlen(target) - 1] == '"'))
		token->herdoc_expand = 0;
	target = remove_quotes_and_track(token, target, size, gc);
	if (token->type == T_HEREDOC)
		token->heredoc_del = target;
	else
		token->filename = target;
}
