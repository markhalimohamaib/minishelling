/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizerv2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:05:06 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/01 17:35:34 by mohamaib         ###   ########.fr       */
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
		// if (cur->quoted)
		//     printf(" | quoted=%d", cur->quoted);
		// if (cur->expand)
		//     printf(" | expands=%d", cur->expand);
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

char	*check_for_fname(token_type type, int *i, char *str, t_gc *gc)
{
	int		size;
	int		j;
	char	*filename;
	char	quote;

	j = 0;
	size = word_len(str, (*i));
	if (type == T_REDIR_IN || type == T_REDIR_OUT || type == T_REDIR_APPEND)
	{
		while (str[(*i)] == ' ' || str[(*i)] == '\t')
			(*i)++;
		filename = gc_malloc(sizeof(char) * (size + 1), gc);
		while (j < size && str[(*i)])
		{
			if (str[(*i)] == '\'' || str[(*i)] == '\"')
			{
				quote = str[(*i)];
				filename[j] = str[(*i)];
				j++;
				(*i)++;
				while (str[(*i)] && str[(*i)] != quote)
				{
					filename[j] = str[(*i)];
					j++;
					(*i)++;
				}
				if (str[(*i)] == quote)
				{
					filename[j] = str[(*i)];
					j++;
					(*i)++;
				}
			}
			else
			{
				filename[j] = str[(*i)];
				j++;
				(*i)++;
			}
		}
		filename[j] = '\0';
		return (filename);
	}
	else
		return (NULL);
}

char	*check_for_heredoc(token_type type, int *i, char *str, t_gc *gc)
{
	int		size;
	int		j;
	char	*heredoc;
	char	quote;

	j = 0;
	size = word_len(str, (*i));
	if (type == T_HEREDOC)
	{
		while (str[(*i)] == ' ' || str[(*i)] == '\t')
			(*i)++;
		heredoc = gc_malloc(sizeof(char) * (size + 1), gc);
		while (j < size && str[(*i)])
		{
			if (str[(*i)] == '\'' || str[(*i)] == '\"')
			{
				quote = str[(*i)];
				heredoc[j] = str[(*i)];
				j++;
				(*i)++;
				while (str[(*i)] && str[(*i)] != quote)
				{
					heredoc[j] = str[(*i)];
					j++;
					(*i)++;
				}
				if (str[(*i)] == quote)
				{
					heredoc[j] = str[(*i)];
					j++;
					(*i)++;
				}
			}
			else
			{
				heredoc[j] = str[(*i)];
				j++;
				(*i)++;
			}
		}
		heredoc[j] = '\0';
		return (heredoc);
	}
	else
		return (NULL);
}

void	add_to_list(t_token **head, token_type type, char *value, int *i,
		char *str, t_gc *gc)
{
	t_token	*new;
	t_token	*tmp;

	new = gc_malloc(sizeof(t_token), gc);
	new->type = type;
	new->value = value;
	new->filename = check_for_fname(new->type, i, str, gc);
	new->heredoc_del = check_for_heredoc(new->type, i, str, gc);
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
}

void	handle_word(char *str, t_token **head, int *i, t_gc *gc)
{
	char	*word;
	int		j;
	int		size;

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

t_token	*tokenize_inp(char *str, t_token **head, t_gc *gc)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (str[i])
	{
		start = 0;
		while (is_space(str[i]))
			i++;
		if (is_metachar(str[i]))
		{
			handle_operator(str, head, &i, gc);
		}
		else if (is_quote(str[i]))
		{
			handle_quotes(str, head, &i, gc);
		}
		else
		{
			handle_word(str, head, &i, gc);
		}
	}
	return (*head);
}

int	main(int argc, char **argv)
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
			continue;
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
		print_token_list(head);
		ast = parse_cmd(&head, &gc);
		print_ast(ast);
		free(line);
		gc_free_all(&gc);
		gc_init(&gc);
	}
	gc_free_all(&gc);
}
