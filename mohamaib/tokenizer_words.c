/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_words.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/05 23:59:59 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_word(char *str, int *i, int size, t_gc *gc)
{
	int		j;
	char	*word;
	char	quote;

	j = 0;
	while (str[(*i)] == ' ' || str[(*i)] == '\t')
		(*i)++;
	word = gc_malloc(sizeof(char) * (size + 1), gc);
	while (j < size && str[(*i)])
	{
		if (str[(*i)] == '\'' || str[(*i)] == '\"')
		{
			quote = str[(*i)];
			word[j++] = str[(*i)++];
			while (str[(*i)] && str[(*i)] != quote)
				word[j++] = str[(*i)++];
			if (str[(*i)] == quote)
				word[j++] = str[(*i)++];
		}
		else
			word[j++] = str[(*i)++];
	}
	word[j] = '\0';
	return (word);
}

void	handle_expansion_char(t_token *token, char *str, int *i,
			int *j, int *w, char *result, char *expan)
{
	int	start_i;

	if (str[*i] == '$' && token->state != IN_SINGLE)
	{
		start_i = *i;
		while (str[*i] && str[*i] != '\'' && str[*i] != '\"'
			&& str[*i] != ' ' && str[*i] != '\t'
			&& str[*i] != '|' && str[*i] != '<' && str[*i] != '>')
		{
			expan[(*w)++] = str[*i];
			result[(*j)++] = str[(*i)++];
		}
		if (*i > start_i)
			token->expand = 1;
	}
	else
		result[(*j)++] = str[(*i)++];
}

char	*remove_quotes_and_track(t_token *token, char *str, int size, t_gc *gc)
{
	int		i;
	int		j;
	int		w;
	char	*result;
	char	*expan;

	i = 0;
	j = 0;
	w = 0;
	token->state = NORMAL;
	token->expand = 0;
	result = gc_malloc(sizeof(char) * (size + 1), gc);
	expan = gc_malloc(sizeof(char) * (size + 1), gc);
	while (str[i])
	{
		if (token->state == NORMAL && (str[i] == '\'' || str[i] == '\"'))
		{
			if (str[i] == '\'')
				token->state = IN_SINGLE;
			else
				token->state = IN_DOUBLE;
		}
		else if (token->state == IN_SINGLE && str[i] == '\'')
			token->state = NORMAL;
		else if (token->state == IN_DOUBLE && str[i] == '\"')
			token->state = NORMAL;
		else
		{
			handle_expansion_char(token, str, &i, &j, &w, result, expan);
			continue ;
		}
		i++;
	}
	result[j] = '\0';
	expan[w] = '\0';
	if (w > 0)
		token->expand_value = expan;
	return (result);
}

void	handle_quoted_word(char *str, t_token **head, int *i, t_gc *gc)
{
	char	*word;
	int		size;
	t_token	*token;

	size = word_len(str, (*i));
	word = extract_word(str, i, size, gc);
	token = create_token(T_WORD, NULL, gc);
	token->value = remove_quotes_and_track(token, word, size, gc);
	add_token_to_list(head, token);
}

void	handle_regular_word(char *str, t_token **head, int *i, t_gc *gc)
{
	char	*word;
	int		j;
	int		size;
	t_token	*token;

	j = 0;
	size = word_len(str, (*i));
	if (size == 0)
		return ;
	word = gc_malloc((sizeof(char) * (size + 1)), gc);
	while (j < size && str[(*i)])
		word[j++] = str[(*i)++];
	word[j] = '\0';
	token = create_token(T_WORD, NULL, gc);
	token->value = remove_quotes_and_track(token, word, size, gc);
	add_token_to_list(head, token);
}

void	process_redir_target(t_token *token, char *str, int *i, t_gc *gc)
{
	int		size;
	char	*target;

	size = word_len(str, (*i));
	target = extract_word(str, i, size, gc);
	target = remove_quotes_and_track(token, target, size, gc);
	if (token->type == T_HEREDOC)
		token->heredoc_del = target;
	else
		token->filename = target;
}
