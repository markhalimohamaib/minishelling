/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:05:06 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/30 00:45:15 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int					is_space(char c);
// int		is_metachar(char c);
int					is_quote(char c);
int					is_redir_start(char c);
int					is_word_char(char c);
// t_token	*handle_word(const char *line, int *i);

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

// int	quo_word_len(char *str, int i)
// {
// 	int	count;
// 	int	end;

// 	// int	quotes;
// 	count = 0;
// 	// quotes = 0;
// 	// while (str[i] != ' ' || str[i] == '\t')
// 	// 	i++;
// 	if (str[i] == '\'')
// 	{
// 		count++;
// 		i++;
// 		while (str[i] != '\'')
// 		{
// 			i++;
// 			count++;
// 		}
// 		return (count + 1);
// 	}
// 	else if (str[i] == '\"')
// 	{
// 		count++;
// 		i++;
// 		while (str[i] != '\"')
// 		{
// 			i++;
// 			count++;
// 		}
// 		return (count + 1);
// 	}
// 	return (count + 1);
// }

int	word_len(char *str, int i)
{
	int		count;
	char	quote;

	count = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i] != ' ' && str[i] != '\t' && str[i] != '|' && str[i] != '>'
		&& str[i] != '<')
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

char	*check_for_fname(token_type type, int *i, char *str)
{
	int		size;
	int		j;
	char	*filename;
	char	quote;

	j = 0;
	size = word_len(str, (*i));
	// if (str[(*i)] == '\'' || str[(*i)] == '\"')
	// 	size = quo_word_len(str, (*i));
	// else
	// 	size = word_len(str, (*i));
	if (type == T_REDIR_IN || type == T_REDIR_OUT || type == T_REDIR_APPEND)
	{
		while (str[(*i)] == ' ' || str[(*i)] == '\t')
			(*i)++;
		filename = malloc(sizeof(char) * (size + 1));
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

char	*check_for_heredoc(token_type type, int *i, char *str)
{
	int		size;
	int		j;
	char	*heredoc;
	char	quote;

	j = 0;
	size = word_len(str, (*i));
	// if (str[(*i)] == '\'' || str[(*i)] == '\"')
	// 	size = quo_word_len(str, (*i));
	// else
	// 	size = word_len(str, (*i));
	if (type == T_HEREDOC)
	{
		while (str[(*i)] == ' ' || str[(*i)] == '\t')
			(*i)++;
		heredoc = malloc(sizeof(char) * (size + 1));
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
		char *str)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
	new->type = type;
	new->value = value;
	// new->quoted = handle_quotes();
	// new->expand = handle_expansions();
	new->filename = check_for_fname(new->type, i, str);
	new->heredoc_del = check_for_heredoc(new->type, i, str);
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

// void	check_n_creat_tokens(t_token **head)
// {
// 	t_token	*tmp;
// 	t_token	*tmp2;

// 	tmp = (*head);
// 	tmp2 = NULL;
// 	while (tmp->next)
// 	{
// 		if ((tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT
// 				|| tmp->type == T_REDIR_APPEND) && tmp->next->type == T_WORD)
// 		{
// 			tmp->filename = tmp->next->value;
// 			tmp2 = tmp->next;
// 			tmp->next = tmp->next->next;
// 			free(tmp2);
// 		}
// 		if (tmp->type == T_HEREDOC && tmp->next->type == T_WORD)
// 		{
// 			tmp->heredoc_del = tmp->next->value;
// 			tmp2 = tmp->next;
// 			tmp->next = tmp->next->next;
// 			free(tmp2);
// 		}
// 		if (tmp->next != NULL)
// 			tmp = tmp->next;
// 	}
// }

// char	*extract_word(char *word, int *i)
// {
// 	int		start;
// 	int		end;
// 	int		state;
// 	char	**extracted;

// 	start = 0;
// 	end = 0;
// 	state = 0;
// 	while(word[start])
// 	{
// 		if(word[start] == '\'')
// 			state = 1;
// 		else if(word[start] == '\"')
// 			state = 2;
// 		while(state == 1 && word[start] == '\'')
// 			end++;
// 	}
// }

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

// void	init_file(t_token **head, int *i, char *str)
// {
// 	char	*filename;
// 	t_token	*tmp;
// 	int		j;
// 	int		size;

// 	while (str[(*i)] == ' ' || str[(*i)] == '\t')
// 		(*i)++;
// 	j = 0;
// 	tmp = (*head);
// 	size = word_len(str, (*i));
// 	filename = malloc(sizeof(char) * (size + 1));
// 	while (j < size && str[(*i)])
// 	{
// 		filename[j] = str[(*i)];
// 		j++;
// 		(*i)++;
// 	}
// 	filename[j] = '\0';
// 	while (tmp->type != T_REDIR_APPEND && tmp->type != T_HEREDOC
// 		&& tmp->type != T_REDIR_IN && tmp->type != T_REDIR_OUT)
// 		tmp = tmp->next;
// 	if (tmp->type == T_HEREDOC)
// 	{
// 		tmp->heredoc_del = filename;
// 		return ;
// 	}
// 	tmp->filename = filename;
// }

void	handle_operator(char *str, t_token **head, int *i)
{
	if (str[(*i)] == '|')
	{
		(*i)++;
		add_to_list(head, T_PIPE, "|", i, str);
		// check_n_creat_tokens(head);
	}
	else if (str[(*i)] == '>' && str[(*i) + 1])
	{
		if (str[(*i) + 1] == '>')
		{
			(*i) += 2;
			add_to_list(head, T_REDIR_APPEND, ">>", i, str);
			// check_n_creat_tokens(head);
			// (*i)++;
			// init_file(head, i, str);
			// while (str[(*i)] != ' ' && str[(*i)] != '\t')
			// 	(*i)++;
		}
		else
		{
			(*i)++;
			add_to_list(head, T_REDIR_OUT, ">", i, str);
			// check_n_creat_tokens(head);e
			// init_file(head, i, str);
		}
	}
	else if (str[(*i)] == '<' && str[(*i) + 1])
	{
		if (str[(*i) + 1] == '<')
		{
			(*i) += 2;
			add_to_list(head, T_HEREDOC, "<<", i, str);
			// check_n_creat_tokens(head);
			// (*i)++;
			// init_file(head, i, str);
		}
		else
		{
			(*i)++;
			add_to_list(head, T_REDIR_IN, "<", i, str);
			// check_n_creat_tokens(head);
			// init_file(head, i, str);
		}
	}
}

void	handle_quotes(char *str, t_token **head, int *i)
{
	char	*quoted_word;
	char	quote;
	int		j;
	int		size;

	// char	*extracted;
	j = 0;
	size = word_len(str, (*i));
	quoted_word = malloc(sizeof(char) * (size + 1));
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
	// extracted = extarct_word(quoted_word, i);
	add_to_list(head, T_WORD, quoted_word, i, str);
}

// void	handle_quotes(char *str, t_token **head, int *i)
// {
// 	char	*quoted_word;
// 	int		j;
// 	// int		size;
// 	int		end;

// 	j = 0;
// 	end = (*i);
// 	if (str[(*i)] == '\'' || str[(*i)] == '\"')
// 	{
// 		end++;
// 		if (str[(*i)] == '\'')
// 			while(str[end] && str[end] == '\'')
// 				end++;
// 		else if (str[(*i)] == '\"')
// 			while(str[end] && str[end] == '\"')
// 				end++;
// 	}
// 	// size = quo_word_len(str, (*i));
// 	quoted_word = malloc(sizeof(char) * ((end - (*i)) + 1));
// 	while (j <= (end - (*i)) && str[(*i)])
// 	{
// 		quoted_word[j] = str[(*i)];
// 		(*i)++;
// 		j++;
// 	}
// 	quoted_word[j] = '\0';
// 	add_to_list(head, T_WORD, quoted_word, i, str);
// }

void	handle_word(char *str, t_token **head, int *i)
{
	char	*word;
	int		j;
	int		size;

	j = 0;
	size = word_len(str, (*i));
	if (size == 0)
		return ;
	word = malloc(sizeof(char) * (size + 1));
	while (j < size && str[(*i)])
	{
		word[j] = str[(*i)];
		j++;
		(*i)++;
	}
	word[j] = '\0';
	add_to_list(head, T_WORD, word, i, str);
}

// int	heredoc_input(char c)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i])
// 	{
// 		if (i == 0 && s[i] == '<')
// 			if (s[i + 1] == '<')
// 				return (1);
// 		i++;
// 	}
// 	return(0);
// }

// int	append_input(char c)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i])
// 	{
// 		if (i == 0 && s[i] == '>')
// 			if (s[i + 1] == '>')
// 				return (1);
// 		i++;
// 	}
// 	return(0);
// }

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

t_token	*create_token(char **all, int *i)
{
}

t_token	*tokenize_inp(char *str, t_token **head)
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
			handle_operator(str, head, &i);
		}
		else if (is_quote(str[i]))
		{
			handle_quotes(str, head, &i);
		}
		else
		{
			handle_word(str, head, &i);
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

	while (1)
	{
		head = NULL;
		line = readline("minishell-> ");
		if (line[0] == '\0')
			break ;
		if (ft_strncmp(line, "exit", 6) == 0)
		{
			free(line);
			// printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		// arr = ft_split_spaces(line);
		// printf("You entered: %s\n", line);
		// tokenize_n_list(arr, &head);
		// get_quotes(head);
		head = tokenize_inp(line, &head);
		print_token_list(head);
		ast = parse_cmd(&head);
		print_ast(ast);
		free(line);
		// free_split(arr);
		freelst(head);
	}
	// free_split(arr);
	// freelst(head);
}
