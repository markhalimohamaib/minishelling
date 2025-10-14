/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 23:00:16 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/14 21:02:20 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC,
	T_EOF
}					token_type;

typedef enum
{
	CMD_NODE,
	PIPE_NODE,
	REDIR_NODE
}					node_type;

typedef struct s_token
{
	token_type		type;
	char			*value;
	// int				quoted;
	// int				expand;
	char			*filename;
	char			*heredoc_del;
	struct s_token	*next;
}					t_token;

typedef struct s_node
{
	node_type		type;
	//for cmd
	char			**cmd;
	t_token			*token;
	//for pipe
	struct s_node	*left;
	struct s_node	*right;
}					t_node;