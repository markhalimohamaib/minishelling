/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maintest.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 23:25:09 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/09 23:37:56 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static const char *token_type_str(token_type t)
{
    switch (t) {
        case T_WORD:         return "TOK_WORD";
		case T_PIPE:         return "TOK_PIPE";
        case T_REDIR_IN:     return "TOK_REDIR_IN";
        case T_REDIR_OUT:    return "TOK_REDIR_OUT";
        case T_REDIR_APPEND: return "TOK_REDIR_APPEND";
        case T_HEREDOC:      return "TOK_HEREDOC";
        case T_EOF:          return "TOK_EOF";
        default:               return "UNKNOWN";
    }
}

void print_tokens(t_token *head)
{
    t_token *cur = head;
    while (cur)
    {
        printf("[%s:\"%s\"", token_type_str(cur->type),
               cur->value ? cur->value : "");
        if (cur->quoted)
            printf(", quoted=%d", cur->quoted);
        if (cur->expand)
            printf(", expands=%d", cur->expand);
        printf("] ");
        cur = cur->next;
    }
    printf("\n");
}

int main(void)
{
    char *line;
	

    while (1)
    {
        line = readline("minishell> ");
        if (!line)
        {
            printf("exit\n");
            break;
		}
        if (*line)
            add_history(line);

        printf("You entered: %s\n", line);
        free(line);
    }
    return 0;
}