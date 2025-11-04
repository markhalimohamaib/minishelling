/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 23:00:16 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/04 23:01:06 by mohamaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct s_gc
{
	t_gc_node	*list;
}	t_gc;

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
	REDIR_NODE,
	EOF_NODE
}					node_type;

typedef enum
{
	NORMAL,
	IN_SINGLE,
	IN_DOUBLE
}					token_state;

typedef struct s_token
{
	token_type		type;
	char			*value;
	char			*expand_value;
	int				state;
	int				expand;
	char			*filename;
	char			*heredoc_del;
	struct s_token	*next;
}					t_token;

typedef struct s_node
{
	node_type		type;
	//for cmd
	char			**cmd;
	char			*filename;
	// t_token			*redirs;
	token_type		redir_type;
	//for pipe
	struct s_node	*left;
	struct s_node	*right;
}					t_node;

typedef struct s_redirs_data
{
	int		i;
	int		count;
	char	**cmd;
	t_node	*node;
	t_node	*redir_node;
	t_token	*tmp;
	t_token	*tmp2;
}					t_redirs_data;
// void print_token_list(t_token *head);

//parser.c
t_node	*parse_cmd(t_token **head, t_gc *gc);
void	print_ast(t_node *root);
void	gc_init(t_gc *gc);
void	*gc_malloc(size_t size, t_gc *gc);
void	gc_free_all(t_gc *gc);
char	*gc_ft_strdup(const char *str, t_gc *gc);