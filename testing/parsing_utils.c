/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 22:59:35 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/12 19:12:32 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_token	*create_token()
// {
// 	t_token *new_tok;

// 	new_tok = malloc(sizeof(t_token));
// 	t_token
// }

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
        printf("[%02d] %-12s | value=\"%s\"", i, token_type_str(cur->type),
               cur->value ? cur->value : "");
        if (cur->quoted)
            printf(" | quoted=%d", cur->quoted);	head = NULL;
        if (cur->expand)
            printf(" | expands=%d", cur->expand);
        printf("\n");

        cur = cur->next;
        i++;
    }
    printf("================\n");
}

// char	*ft_strdup(const char *str)
// {
// 	char	*dup;
// 	int		i;

// 	i = 0;
// 	dup = (char *)malloc(sizeof (char) * (ft_strlen(str) + 1));
// 	if (!dup)
// 		return (0);
// 	while (str[i])
// 	{
// 		dup[i] = str[i];
// 		i++;
// 	}
// 	dup[i] = '\0';
// 	return (dup);
// }

char	*create_dup(const char *str, int index, int isquoted)
{
	char	*dup;
	int		i;
	int		j;

	i = index;
	j = index;
	
	if (isquoted == 1)
	{
		while(str[j] != '\'')
			j++;
		j--;
	}
	else if(isquoted == 2)
	{
		while(str[j] != '\"')
			j++;
		j--;
	}
	else
	{
		while(str[j] != ' ' && str[j] != '|' && str[j] != '<' && str[j] != '>' && str[j] != '\t')
			j++;
		j--;
	}
	dup = (char *)malloc(sizeof(char) * ((j - i) + 1));
	if (!dup)
		return (0);
	while (str[i] && i <= j)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	add_to_list(t_token **head, token_type type, char *value, int quoted, int expand)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
	new->type = type;
	new->value = ft_strdup(value);
	new->quoted = quoted;
	new->expand = expand;
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

void	split_tokenize(char *line, t_token **head)
{
	int		i;
	int		j;
	// t_token	*head;
	t_token	*new_token;
	char	*value;

	// new_token = malloc(sizeof(t_token));
	// new_token = NULL;
	// head = NULL;
	i = 0;
	j = 0;
	while (line[i])
	{
		// while(line[i] == ' ' || line[i] == '\t')
		// 	i++;
		if (line[i] == '|')
		{
			add_to_list(head, T_PIPE, "|", 0, 0);
			// i++;
		}
		else if (line[i] == '<')
		{
			if (line[i + 1] == '<')
			{
				add_to_list(head, T_HEREDOC, "<<", 0, 0);
				// i += 2;
			}
			else
			{
				add_to_list(head, T_REDIR_OUT, "<", 0, 0);
				// i++;
			}
		}
		else if (line[i] == '\'' || line[i] == '\"')
		{
			if (line[i] == '\'')
			{
				j = i;
				while (line[j] != '\'')
					j++;
				j--;
				value = ft_substr(line, i, j - i);
				add_to_list(head, T_WORD, value, 1, 0);
				// i = j + 2;
			}
			if (line[i] == '\"')
			{
				j = i;
				while (line[j] != '\"')
					j++;
				j--;
				value = ft_substr(line, i, j - i);
				add_to_list(head, T_WORD, value, 2, 0);
				i = j + 2;
			}
		}
		i++;
	}
}
// 'echo' "hello" | 'world' > "infile" < "out"
int main (int argc, char **argv)
{
	char *line;
	t_token *head;

	head = NULL;
	while(1)
	{
		line = readline("minishell-> ");
		if (!line)
        {
            printf("exit\n");
            break;
		}
        if (*line)
            add_history(line);

		
        printf("You entered: %s\n", line);
		split_tokenize(line, &head);
		print_token_list(head);
        free(line);
	}
}