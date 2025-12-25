/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_words.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/23 21:36:08 by mohamaib         ###   ########.fr       */
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

static void	handle_process_target(t_token *token, char *str, int *i,
		t_redir_processor *pro)
{
	int	start_i;

	if (str[*i] == '$' && token->state != IN_SINGLE)
	{
		start_i = *i;
		while (str[*i] && str[*i] != '\'' && str[*i] != '\"' && str[*i] != ' '
			&& str[*i] != '\t' && str[*i] != '|' && str[*i] != '<'
			&& str[*i] != '>')
		{
			pro->result[pro->j++] = str[(*i)++];
		}
		if (*i > start_i)
			token->expand_line = 1;
	}
	else
		pro->result[pro->j++] = str[(*i)++];
}

static void	set_token_state(t_redir_processor *pro, t_token **token, char **str)
{
	int	i;

	i = 0;
	while ((*str)[i])
	{
		if ((*token)->state == NORMAL && ((*str)[i] == '\''
			|| (*str)[i] == '\"'))
		{
			if ((*str)[i] == '\'')
				(*token)->state = IN_SINGLE;
			else
				(*token)->state = IN_DOUBLE;
		}
		else if ((*token)->state == IN_SINGLE && (*str)[i] == '\'')
			(*token)->state = NORMAL;
		else if ((*token)->state == IN_DOUBLE && (*str)[i] == '\"')
			(*token)->state = NORMAL;
		else
		{
			handle_process_target(*token, *str, &i, pro);
			continue ;
		}
		i++;
	}
}

char	*remove_quotes_and_track(t_token *token, char *str, int size, t_gc *gc)
{
	t_redir_processor	*pro;

	pro = gc_malloc(sizeof(t_redir_processor), gc);
	pro->j = 0;
	token->state = NORMAL;
	token->expand_line = 0;
	pro->result = gc_malloc(sizeof(char) * (size + 1), gc);
	set_token_state(pro, &token, &str);
	pro->result[pro->j] = '\0';
	return (pro->result);
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
	token->origin_val = gc_ft_strdup(word, gc);
	token->segment = build_segment(token, gc);
	token->value = remove_quotes_and_track(token, word, size, gc);
	add_token_to_list(head, token);
}
