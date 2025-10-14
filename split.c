/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 00:32:35 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/14 21:34:13 by mohamaib         ###   ########.fr       */
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
		free (temp);
	}
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static const char *token_type_str(token_type type)
{
    switch (type)
    {
        case T_WORD:         return "WORD";
        case T_PIPE:         return "PIPE";
        case T_REDIR_IN:     return "REDIR_IN";
        case T_REDIR_OUT:    return "REDIR_OUT";
        case T_REDIR_APPEND: return "REDIR_APPEND";
        case T_HEREDOC:      return "HEREDOC";
        case T_EOF:          return "EOF";
        default:               return "UNKNOWN";
    }
}

void print_token_list(t_token *head)
{
    t_token *cur = head;
    int i = 0;

    printf("=== TOKENS ===\n");
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
    printf("================\n");
}

// void	get_quotes(t_token **head)
// {
// 	t_token *tmp;

// 	tmp = (*head);
// 	while (tmp->next)
// 	{
// 		if (tmp.)	
// 	}
// }

static int	is_sep(char sep)
{
	return (sep == ' ' || sep == '\t');
}

static int	count_words(const char *s)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (!is_sep(s[i]) && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (is_sep(s[i]))
			in_word = 0;
		i++;
	}
	return (count);
}

static char	*allocate(const char *s, int start, int len)
{
	char	*ptr;

	ptr = malloc(len + 1);
	if (!ptr)
		return (NULL);
	for (int i = 0; i < len; i++)
		ptr[i] = s[start + i];
	ptr[len] = '\0';
	return (ptr);
}

char	**ft_split_spaces(const char *s)
{
	int		total;
	char	**out;
	int		i;
	int		start;
	int		j;

	if (!s)
		return (NULL);
	total = count_words(s);
	out = malloc(sizeof(char *) * (total + 1));
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (is_sep(s[i]))
			i++;
		if (!s[i])
			break ;
		start = i;
		while (s[i] && !is_sep(s[i]))
			i++;
		out[j++] = allocate(s, start, i - start);
	}
	out[j] = NULL;
	return (out);
}

void	check_n_creat_tokens(t_token **head)
{
	t_token	*tmp;
	t_token	*tmp2;

	tmp = (*head);
	tmp2 = NULL;
	while (tmp->next)
	{
		if ((tmp->type == T_REDIR_IN || tmp->type == T_REDIR_OUT || 
			tmp->type == T_REDIR_APPEND) && tmp->next->type == T_WORD)
		{
			tmp->filename = tmp->next->value;
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			free(tmp2);
		}
		if (tmp->type == T_HEREDOC && tmp->next->type == T_WORD)
		{
			tmp->heredoc_del = tmp->next->value;
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			free(tmp2);
		}
		if (tmp->next != NULL)
			tmp = tmp->next;
	}
}

void	check_n_creat_cmds(t_token **head)
{
	t_node	*new_node;
	t_token	*tmp;
	int		i;

	i = 0;
	tmp = (*head);
	new_node = malloc(sizeof(t_node));
	new_node->cmd = NULL;
	while(tmp->next)
	{
		while (tmp->type == T_WORD && tmp->next != NULL)
		{
			new_node->cmd[i] = ft_strdup(tmp->next->value);
			tmp = tmp->next;
			i++;
		}
		if (tmp->type == T_PIPE)
		{
			
		}
	}
}

void	add_to_list(t_token **head, token_type type, char *value)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
	new->type = type;
	new->value = value;
	// new->quoted = quoted;
	// new->expand = expand;
	new->filename = NULL;
	new->heredoc_del = NULL;
	new->next = NULL;
	if (!(*head))
	{
		(*head) = new;
		return ;
	}
	tmp = *head;
	while(tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	pipe_input(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (i == 0 && s[i] == '|')
			return (1);
		i++;
	}
	return(0);
}

int	redir_input_in(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (i == 0 && s[i] == '<')
			return (1);
		i++;
	}
	return(0);
}

int	redir_input_out(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (i == 0 && s[i] == '>')
			return (1);
		i++;
	}
	return(0);
}

int	heredoc_input(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (i == 0 && s[i] == '<')
			if (s[i + 1] == '<')
				return (1);
		i++;
	}
	return(0);
}

int	append_input(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (i == 0 && s[i] == '>')
			if (s[i + 1] == '>')
				return (1);
		i++;
	}
	return(0);
}

token_type	ret_tok(char *small_arr)
{
	if (pipe_input(small_arr))
		return (T_PIPE);
	if (heredoc_input(small_arr))
		return (T_HEREDOC);
	if (append_input(small_arr))
		return (T_REDIR_APPEND);
	if (redir_input_in(small_arr))
		return (T_REDIR_IN);
	if (redir_input_out(small_arr))
		return (T_REDIR_OUT);
	return (T_WORD);
}

void	tokenize_n_list(char **arr, t_token **head)
{
	int		i;
	int		j;
	
	i = 0;
	j = 0;
	while (arr[i])
	{
		add_to_list(head, ret_tok(arr[i]), arr[i]);
		i++;
	}
	check_n_creat_tokens(head);
	check_n_creat_cmds(head);
}

// 'echo' "hello" | 'world' > "infile" < "out"
// echo hello | cat -e > infile
//   cat hello >> infile.txt | echo hello << outfile > > < << infile outfil | 
// ls -la < infile.txt | wc -l > outfile.txt
int main (int argc, char **argv)
{
	char *line;
	char **arr;
	t_token *head;

	while(1)
	{
		head = NULL;
		line = readline("minishell-> ");
		if (ft_strncmp(line, "exit", 6) == 0)
        {
			free(line);
            // printf("exit\n");
            break;
		}
        if (*line)
            add_history(line);
		arr = ft_split_spaces(line);
        // printf("You entered: %s\n", line);
		tokenize_n_list(arr, &head);
		// get_quotes(head);
		print_token_list(head);
        free(line);
		free_split(arr);
		freelst(head);
	}
	// free_split(arr);
	// freelst(head);
}
