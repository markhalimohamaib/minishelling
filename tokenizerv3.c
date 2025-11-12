/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizerv3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:05:06 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/07 21:44:01 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freelst(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
}

static const char	*token_type_str(token_type type)
{
	switch (type)
	{
	case T_WORD:
		return ("WORD");
	case T_PIPE:
		return ("PIPE");
	case T_REDIR_IN:
		return ("REDIR_IN");
	case T_REDIR_OUT:
		return ("REDIR_OUT");
	case T_REDIR_APPEND:
		return ("REDIR_APPEND");
	case T_HEREDOC:
		return ("HEREDOC");
	case T_EOF:
		return ("EOF");
	default:
		return ("UNKNOWN");
	}
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
		printf("[%02d] %-12s | value = \"%s\"", i, token_type_str(cur->type),
			cur->value ? cur->value : "");
		if (cur->state)
			printf(" | state=%d", cur->state);
		if (cur->expand)
			printf(" | expands=%d", cur->expand);
		if ((cur->type == T_WORD && cur->expand_value && cur->expand_value == NULL)
			|| (cur->type == T_WORD && cur->expand_value && cur->expand_value[0] != '\0'))
			printf(" | expand_val=%s", cur->expand_value);
		if (cur->filename)
			printf(" | filename = %s", cur->filename);
		if (cur->heredoc_del)
			printf(" | heredoc_del = %s", cur->heredoc_del);
		printf("\n");
		cur = cur->next;
		i++;
	}
	printf("================\n\n");
}

// ---------------------------------------------------------------------------------

int	word_len(char *str, int i)
{
	int		count;
	char	quote;

	count = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '|'
		&& str[i] != '>' && str[i] != '<')
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i];
			count++;
			i++;
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

// char	*check_for_fname(token_type type, int *i, char *str, t_gc *gc)
// {
// 	int		size;
// 	int		j;
// 	char	*filename;
// 	char	quote;

// 	j = 0;
// 	size = word_len(str, (*i));
// 	if (type == T_REDIR_IN || type == T_REDIR_OUT || type == T_REDIR_APPEND)
// 	{
// 		while (str[(*i)] == ' ' || str[(*i)] == '\t')
// 			(*i)++;
// 		filename = gc_malloc(sizeof(char) * (size + 1), gc);
// 		while (j < size && str[(*i)])
// 		{
// 			if (str[(*i)] == '\'' || str[(*i)] == '\"')
// 			{
// 				quote = str[(*i)];
// 				filename[j] = str[(*i)];
// 				j++;
// 				(*i)++;
// 				while (str[(*i)] && str[(*i)] != quote)
// 				{
// 					filename[j] = str[(*i)];
// 					j++;
// 					(*i)++;
// 				}
// 				if (str[(*i)] == quote)
// 				{
// 					filename[j] = str[(*i)];
// 					j++;
// 					(*i)++;
// 				}
// 			}
// 			else
// 			{
// 				filename[j] = str[(*i)];
// 				j++;
// 				(*i)++;
// 			}
// 		}
// 		filename[j] = '\0';
// 		return (filename);
// 	}
// 	else
// 		return (NULL);
// }

// char	*check_for_heredoc(token_type type, int *i, char *str, t_gc *gc)
// {
// 	int		size;
// 	int		j;
// 	char	*heredoc;
// 	char	quote;

// 	j = 0;
// 	size = word_len(str, (*i));
// 	if (type == T_HEREDOC)
// 	{
// 		while (str[(*i)] == ' ' || str[(*i)] == '\t')
// 			(*i)++;
// 		heredoc = gc_malloc(sizeof(char) * (size + 1), gc);
// 		while (j < size && str[(*i)])
// 		{
// 			if (str[(*i)] == '\'' || str[(*i)] == '\"')
// 			{
// 				quote = str[(*i)];
// 				heredoc[j] = str[(*i)];
// 				j++;
// 				(*i)++;
// 				while (str[(*i)] && str[(*i)] != quote)
// 				{
// 					heredoc[j] = str[(*i)];
// 					j++;
// 					(*i)++;
// 				}
// 				if (str[(*i)] == quote)
// 				{
// 					heredoc[j] = str[(*i)];
// 					j++;
// 					(*i)++;
// 				}
// 			}
// 			else
// 			{
// 				heredoc[j] = str[(*i)];
// 				j++;
// 				(*i)++;
// 			}
// 		}
// 		heredoc[j] = '\0';
// 		return (heredoc);
// 	}
// 	else
// 		return (NULL);
// }

char	*process_quotes_and_expand(t_token *node, char *str, int size, t_gc *gc)
{
	int		i;
	int		j;
	int		w;
	char	*extracted;
	char	*extrac_expan;
	int		start_i;

	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	w = 0;
	node->state = NORMAL;
	node->expand = 0;
	extracted = gc_malloc(sizeof(char) * (size + 1), gc);
	extrac_expan = gc_malloc(sizeof(char) * (size + 1), gc);
	while (str[i])
	{
		if (node->state == NORMAL)
		{
			if (str[i] == '\'')
			{
				node->state = IN_SINGLE;
				i++;
			}
			else if (str[i] == '\"')
			{
				node->state = IN_DOUBLE;
				i++;
			}
			else if (str[i] == '$')
			{
				start_i = i;
				while (str[i] && str[i] != '\'' && str[i] != '\"'
					&& str[i] != ' ' && str[i] != '\t' && str[i] != '|'
					&& str[i] != '<' && str[i] != '>')
				{
					extrac_expan[w++] = str[i];
					extracted[j++] = str[i];
					i++;
				}
				if (i > start_i)
					node->expand = 1;
			}
			else
				extracted[j++] = str[i++];
		}
		else if (node->state == IN_SINGLE)
		{
			if (str[i] == '\'')
			{
				node->state = NORMAL;
				i++;
			}
			else
				extracted[j++] = str[i++];
		}
		else if (node->state == IN_DOUBLE)
		{
			if (str[i] == '$')
			{
				start_i = i;
				while (str[i] && str[i] != '\'' && str[i] != '\"'
					&& str[i] != ' ' && str[i] != '\t' && str[i] != '|'
					&& str[i] != '<' && str[i] != '>')
				{
					extrac_expan[w++] = str[i];
					extracted[j++] = str[i];
					i++;
				}
				if (i > start_i)
					node->expand = 1;
			}
			else if (str[i] == '\"')
			{
				node->state = NORMAL;
				i++;
			}
			else
				extracted[j++] = str[i++];
		}
	}
	extracted[j] = '\0';
	extrac_expan[w] = '\0';
	if (w > 0)
		node->expand_value = extrac_expan;
	return (extracted);
}

char	*extract_word_with_quotes(char *str, int *i, int size, t_gc *gc)
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

char	*check_for_fname(t_token *new, int *i, char *str, t_gc *gc)
{
	int		size;
	char	*filename;

	if (new->type != T_REDIR_IN &&new->type != T_REDIR_OUT
		&&new->type != T_REDIR_APPEND)
		return (NULL);
	size = word_len(str, (*i));
	filename = extract_word_with_quotes(str, i, size, gc);
	filename = process_quotes_and_expand(new, filename, size, gc);
	return (filename);
}

char	*check_for_heredoc(t_token *new, int *i, char *str, t_gc *gc)
{
	int		size;
	char	*heredoc;

	if (new->type != T_HEREDOC)
		return (NULL);
	size = word_len(str, (*i));
	heredoc = extract_word_with_quotes(str, i, size, gc);
	heredoc = process_quotes_and_expand(new, heredoc, size, gc);
	return (heredoc);
}

void	get_state(t_token *node, int size, t_gc *gc)
{
	int		i;
	int		j;
	int		w;
	char	*extracted;
	char	*extrac_expan;
	int		start_w;
	int		start_i;

	j = 0;
	i = 0;
	w = 0;
	node->state = NORMAL;
	node->expand = 0;
	extracted = gc_malloc(sizeof(char) * (size + 1), gc);
	extrac_expan = gc_malloc(sizeof(char) * (size + 1), gc);
	while (node->value[i])
	{
		if (node->state == NORMAL)
		{
			if (node->value[i] == '\'')
			{
				node->state = IN_SINGLE;
				i++;
			}
			else if (node->value[i] == '\"')
			{
				node->state = IN_DOUBLE;
				i++;
			}
			else if (node->value[i] == '$')
			{
				start_w = w;
				start_i = i;
				while (node->value[i] && node->value[i] != '\''
					&& node->value[i] != '\"' && node->value[i] != ' '
					&& node->value[i] != '\t' && node->value[i] != '|'
					&& node->value[i] != '<' && node->value[i] != '>')
				{
					extrac_expan[w++] = node->value[i];
					extracted[j++] = node->value[i];
					i++;
				}
				if (i > start_i)
					node->expand = 1;
			}
			else
				extracted[j++] = node->value[i++];
		}
		else if (node->state == IN_SINGLE)
		{
			if (node->value[i] == '\'')
			{
				node->state = NORMAL;
				i++;
			}
			else
				extracted[j++] = node->value[i++];
		}
		else if (node->state == IN_DOUBLE)
		{
			if (node->value[i] == '$')
			{
				start_w = w;
				start_i = i;
				while (node->value[i] && node->value[i] != '\''
					&& node->value[i] != '\"' && node->value[i] != ' '
					&& node->value[i] != '\t' && node->value[i] != '|'
					&& node->value[i] != '<' && node->value[i] != '>')
				{
					extrac_expan[w++] = node->value[i];
					extracted[j++] = node->value[i];
					i++;
				}
				if (i > start_i)
					node->expand = 1;
			}
			else if (node->value[i] == '\"')
			{
				node->state = NORMAL;
				i++;
			}
			else
				extracted[j++] = node->value[i++];
		}
	}
	extracted[j] = '\0';
	extrac_expan[w] = '\0';
	node->value = extracted;
	node->expand_value = extrac_expan;
}

// void	get_state(t_token *node, int size, t_gc *gc)
// {
// 	int			i;
// 	int			j;
// 	char		*extracted;

// 	j = 0;
// 	i = 0;
// 	extracted = gc_malloc(sizeof(char) * (size + 1), gc);
// 	while(node->value[i])
// 	{
// 		if (node->state == NORMAL)
// 		{
// 			if (node->value[i] == '\'')
// 				node->state = IN_SINGLE;
// 			else if (node->value[i] == '\"')
// 				node->state = IN_DOUBLE;
// 			else if (node->value[i] == '$')
// 			{
// 				extracted[j++] = node->value[i];
// 				node->expand = 1;
// 			}
// 			else
// 				extracted[j++] = node->value[i];
// 		}
// 		else if (node->state == IN_SINGLE)
// 		{
// 			if (node->value[i] == '$')
// 				extracted[j++] = node->value[i];
// 			else if (node->value[i] == '\'')
// 				node->state = NORMAL;
// 			else
// 				extracted[j++] = node->value[i];
// 		}
// 		else if (node->state == IN_DOUBLE)
// 		{
// 			if (node->value[i] == '$')
// 			{
// 				extracted[j++] = node->value[i];
// 				node->expand = 1;
// 			}
// 			else if (node->value[i] == '\"')
// 				node->state = NORMAL;
// 			else
// 				extracted[j++] = node->value[i];
// 		}
// 		i++;
// 	}
// 	extracted[j++] = '\0';
// 	node->value = extracted;
// }

// void	extract_word(t_token **head)
// {
// 	// char	*extracted;
// 	char	quote;
// 	int		i;
// 	// int		state;
// 	t_token	*tmp;

// 	tmp = (*head);
// 	i = 0;
// 	// state = 0;
// 	while(tmp->next)
// 		tmp = tmp->next;
// 	tmp->state = get_state(tmp->value);
// 	if(tmp->state == IN_SINGLE)
// 		tmp->expand = 0;
// 	else
// 		tmp->expand = 1;
// }

void	add_to_list(t_token **head, token_type type, char *value, int *i,
		char *str, t_gc *gc)
{
	t_token	*new;
	t_token	*tmp;

	new = gc_malloc(sizeof(t_token), gc);
	new->type = type;
	new->value = value;
	new->state = NORMAL;
	new->expand = 0;
	new->filename = check_for_fname(new, i, str, gc);
	new->heredoc_del = check_for_heredoc(new, i, str, gc);
	new->next = NULL;
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

int	is_space(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	pipe_input(char c)
{
	if (c == '|')
		return (1);
	return (0);
}

int	redir_input_in(char c)
{
	if (c == '<')
		return (1);
	return (0);
}

int	redir_input_out(char c)
{
	if (c == '>')
		return (1);
	return (0);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

void	handle_operator(char *str, t_token **head, int *i, t_gc *gc)
{
	if (str[(*i)] == '|')
	{
		(*i)++;
		add_to_list(head, T_PIPE, "|", i, str, gc);
	}
	else if (str[(*i)] == '>' && str[(*i) + 1])
	{
		if (str[(*i) + 1] == '>')
		{
			(*i) += 2;
			add_to_list(head, T_REDIR_APPEND, ">>", i, str, gc);
		}
		else
		{
			(*i)++;
			add_to_list(head, T_REDIR_OUT, ">", i, str, gc);
		}
	}
	else if (str[(*i)] == '<' && str[(*i) + 1])
	{
		if (str[(*i) + 1] == '<')
		{
			(*i) += 2;
			add_to_list(head, T_HEREDOC, "<<", i, str, gc);
		}
		else
		{
			(*i)++;
			add_to_list(head, T_REDIR_IN, "<", i, str, gc);
		}
	}
}

void	handle_quotes(char *str, t_token **head, int *i, t_gc *gc)
{
	char	*quoted_word;
	char	quote;
	int		j;
	int		size;
	t_token	*tmp;

	j = 0;
	size = word_len(str, (*i));
	quoted_word = gc_malloc((sizeof(char) * (size + 1)), gc);
	while (j < size && str[(*i)])
	{
		if (str[(*i)] == '\'' || str[(*i)] == '\"')
		{
			quote = str[(*i)];
			quoted_word[j] = str[(*i)];
			j++;
			(*i)++;
			while (str[(*i)] && str[(*i)] != quote)
			{
				quoted_word[j] = str[(*i)];
				j++;
				(*i)++;
			}
			if (str[(*i)] == quote)
			{
				quoted_word[j] = str[(*i)];
				j++;
				(*i)++;
			}
		}
		else
		{
			quoted_word[j] = str[(*i)];
			(*i)++;
			j++;
		}
	}
	quoted_word[j] = '\0';
	add_to_list(head, T_WORD, quoted_word, i, str, gc);
	tmp = (*head);
	while (tmp && tmp->next)
		tmp = tmp->next;
	get_state(tmp, size, gc);
	// extract_word(head);
}

void	handle_word(char *str, t_token **head, int *i, t_gc *gc)
{
	char	*word;
	int		j;
	int		size;
	t_token	*tmp;

	j = 0;
	size = word_len(str, (*i));
	if (size == 0)
		return ;
	word = gc_malloc((sizeof(char) * (size + 1)), gc);
	while (j < size && str[(*i)])
	{
		word[j] = str[(*i)];
		j++;
		(*i)++;
	}
	word[j] = '\0';
	add_to_list(head, T_WORD, word, i, str, gc);
	tmp = (*head);
	while (tmp && tmp->next)
		tmp = tmp->next;
	get_state(tmp, size, gc);
}

token_type	is_metachar(char c)
{
	if (pipe_input(c))
		return (1);
	if (redir_input_in(c))
		return (1);
	if (redir_input_out(c))
		return (1);
	return (0);
}

// int	check_syntax(t_token *head)
// {
// 	t_token	*tmp;

// 	tmp = head;
// 	while (tmp)
// 	{
// 		if ((tmp->type == T_PIPE && !(tmp->next))
// 			|| (tmp->type == T_REDIR_APPEND && !(tmp->next))
// 			|| (tmp->type == T_REDIR_IN && !(tmp->next))
// 			|| (tmp->type == T_REDIR_OUT && !(tmp->next))
// 			|| (tmp->type == T_HEREDOC && !(tmp->next)))
// 			return (1);
// 		else if ((tmp->type == T_PIPE && tmp->next == NULL)
// 				|| (tmp->type == T_REDIR_IN && tmp->next == NULL)
// 				|| (tmp->type == T_REDIR_OUT && tmp->next == NULL)
// 				|| (tmp->type == T_REDIR_APPEND && tmp->next == NULL)
// 				|| (tmp->type == T_HEREDOC && tmp->next == NULL))
// 			return (1);
// 		tmp = tmp->next;
// 	}
// }

t_token	*tokenize_inp(char *str, t_token **head, t_gc *gc)
{
	int	i;

	i = 0;
	while (str[i])
	{
		while (is_space(str[i]))
			i++;
		if (is_metachar(str[i]))
			handle_operator(str, head, &i, gc);
		else if (is_quote(str[i]))
			handle_quotes(str, head, &i, gc);
		else
			handle_word(str, head, &i, gc);
	}
	return (*head);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;
	char	**arr;
	t_token	*head;
	t_node	*ast;
	t_gc	gc;

	gc_init(&gc);
	while (1)
	{
		head = NULL;
		line = readline("minishell-> ");
		if (!line)
		{
			free(line);
			gc_free_all(&gc);
			break ;
		}
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		if (ft_strncmp(line, "exit", 4) == 0)
		{
			free(line);
			gc_free_all(&gc);
			break ;
		}
		if (*line)
			add_history(line);
		head = tokenize_inp(line, &head, &gc);
		// if (check_syntax(head))
		// {
		// 	free(line);
		// 	continue;
		// }
		print_token_list(head);
		ast = parse_cmd(&head, &gc);
		print_ast(ast);
		free(line);
		gc_free_all(&gc);
		gc_init(&gc);
		printf("%s\n", env[1]);
	}
	gc_free_all(&gc);
}
