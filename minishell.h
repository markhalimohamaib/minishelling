/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 23:00:16 by mohamaib          #+#    #+#             */
/*   Updated: 2025/11/07 20:23:03 by markhali         ###   ########.fr       */
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
}						t_gc_node;

typedef struct s_gc
{
	t_gc_node			*list;
}						t_gc;

typedef enum
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC,
	T_EOF
}						token_type;

typedef enum
{
	CMD_NODE,
	PIPE_NODE,
	REDIR_NODE,
	EOF_NODE
}						node_type;

typedef enum
{
	NORMAL,
	IN_SINGLE,
	IN_DOUBLE
}						token_state;

typedef struct s_token
{
	token_type			type;
	char				*value;
	char				*expand_value;
	int					state;
	int					expand;
	char				*filename;
	char				*heredoc_del;
	struct s_token		*next;
}						t_token;

typedef struct s_node
{
	node_type			type;
	char				**cmd;
	// char				**full_dir;
	char				*filename;
	token_type			redir_type;
	struct s_node		*left;
	struct s_node		*right;
}						t_node;

typedef struct s_redirs_data
{
	int					i;
	int					count;
	char				**cmd;
	t_node				*node;
	t_node				*redir_node;
	t_token				*tmp;
	t_token				*tmp2;
}						t_redirs_data;

// tokenizerv3.c
t_token					*tokenize_inp(char *str, t_token **head, t_gc *gc);
token_type				is_metachar(char c);
void					handle_word(char *str, t_token **head, int *i,
							t_gc *gc);
void					handle_quotes(char *str, t_token **head, int *i,
							t_gc *gc);
void					handle_operator(char *str, t_token **head, int *i,
							t_gc *gc);
int						is_quote(char c);
void					freelst(t_token *head);
static const char		*token_type_str(token_type type);
void					print_token_list(t_token *head);
int						word_len(char *str, int i);
char					*process_quotes_and_expand(t_token *node, char *str,
							int size, t_gc *gc);
char					*extract_word_with_quotes(char *str, int *i, int size,
							t_gc *gc);
char					*check_for_fname(t_token *new, int *i, char *str,
							t_gc *gc);
char					*check_for_heredoc(t_token *new, int *i, char *str,
							t_gc *gc);
void					get_state(t_token *node, int size, t_gc *gc);
void					add_to_list(t_token **head, token_type type,
							char *value, int *i, char *str, t_gc *gc);
int						is_space(char c);
int						pipe_input(char c);
int						redir_input_in(char c);
int						redir_input_out(char c);

// parserv2.c
static void				print_indent(int depth);
static void				print_redirs(t_token *redir, int depth);
static void				print_ast_recursive(t_node *node, int depth);
void					print_ast(t_node *root);
void					print_ast_compact(t_node *node);
void					print_node(t_node *node);
int						count_until_pipe(t_token **head);
char					**create_cmd_from_tokens(t_token **head, t_gc *gc);
t_node					*create_nodes(t_token **head, char **cmd, t_gc *gc);
t_node					*create_redir_node(token_type type, char *filename,
							t_node *left, t_gc *gc);
void					skip_to_pipe(t_token **head);
t_token					*find_last_redir(t_token *start);
t_node					*wrap_redirs(t_token *redir, t_node *node, t_gc *gc);
t_node					*parse_smpl_cmd(t_token **head, t_gc *gc);
t_node					*parse_cmd(t_token **head, t_gc *gc);

// gc.c
void					gc_init(t_gc *gc);
void					*gc_malloc(size_t size, t_gc *gc);
void					gc_free_all(t_gc *gc);
char					*gc_ft_strdup(const char *str, t_gc *gc);