/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 23:00:16 by mohamaib          #+#    #+#             */
/*   Updated: 2025/10/24 17:13:34 by markhali         ###   ########.fr       */
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


void	gc_init(t_gc *gc);
void	*gc_malloc(t_gc *gc, size_t size);
void	gc_free_all(t_gc *gc);