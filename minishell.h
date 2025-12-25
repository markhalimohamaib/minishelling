/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: markhali <markhali@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 23:00:16 by mohamaib          #+#    #+#             */
/*   Updated: 2025/12/25 18:40:38 by markhali         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_signal;

typedef struct s_env
{
	char						*key;
	char						*value;
	struct s_env				*next;
}								t_env;

typedef enum e_builtin_type
{
	BLT_NONE,
	BLT_ECHO,
	BLT_CD,
	BLT_PWD,
	BLT_EXPORT,
	BLT_UNSET,
	BLT_ENV,
	BLT_EXIT
}								t_builtin_type;

typedef struct s_gc_node
{
	void						*ptr;
	struct s_gc_node			*next;
}								t_gc_node;

typedef struct s_gc
{
	t_gc_node					*list;
}								t_gc;

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC,
	T_EOF
}								t_token_type;

typedef enum e_node_type
{
	CMD_NODE,
	PIPE_NODE,
	REDIR_NODE,
	EOF_NODE
}								t_node_type;

typedef enum e_token_state
{
	NORMAL,
	IN_SINGLE,
	IN_DOUBLE
}								t_token_state;

typedef struct s_segment
{
	char						*str;
	t_token_state				seg_state;
	int							expands;
}								t_segment;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	char						*origin_val;
	t_segment					*segment;
	int							seg_count;
	int							state;
	int							expand_line;
	char						*filename;
	char						*heredoc_del;
	int							herdoc_expand;
	struct s_token				*next;
}								t_token;

typedef struct s_node
{
	int							pid1;
	int							pid2;
	int							pipefd[2];
	int							file_fd;
	int							status;
	t_node_type					type;
	char						**cmd;
	char						**full_dir;
	char						*filename;
	t_token_type				redir_type;
	struct s_node				*left;
	struct s_node				*right;
	t_builtin_type				builtin;
	char						*delim;
	int							heredoc_expand;
	int							heredoc_fd;
}								t_node;

typedef struct s_val_full_init
{
	int							i;
	int							j;
	int							z;
	char						*expand_val;
	char						*leading_val;
	char						*trailing_val;
	char						*full;
	char						*full_all;
}								t_val_full_init;

typedef struct s_redir_processor
{
	int							i;
	int							j;
	char						*result;
}								t_redir_processor;

typedef struct s_child_vars
{
	int							i;
	char						*cmd;
	char						**envp;
	char						*path;
	char						**dirs;
	int							check_result;
}								t_child_vars;

typedef struct s_heredoc_vars
{
	int							*p;
	const char					*delim;
	int							expand;
	t_env						**env;
	t_gc						*gc;
}								t_heredoc_vars;

/* gc.c */
void							gc_init(t_gc *gc);
void							*gc_malloc(size_t size, t_gc *gc);
void							gc_free_all(t_gc *gc);
char							*gc_ft_strdup(const char *str, t_gc *gc);
char							*gc_ft_strjoin(char const *s1, char const *s2,
									t_gc *gc);

/* tokenizer.c */
int								word_len(char *str, int i);
void							add_token_to_list(t_token **head, t_token *new);
t_token							*create_token(t_token_type type, char *value,
									t_gc *gc);
void							handle_operator(char *str, t_token **head,
									int *i, t_gc *gc);
t_token							*tokenize_input(char *str, t_gc *gc);

/* tokenizer_words.c */
char							*extract_word(char *str, int *i, int size,
									t_gc *gc);
char							*remove_quotes_and_track(t_token *token,
									char *str, int size, t_gc *gc);
void							handle_quoted_word(char *str, t_token **head,
									int *i, t_gc *gc);
void							handle_regular_word(char *str, t_token **head,
									int *i, t_gc *gc);
void							process_redir_target(t_token *token, char *str,
									int *i, t_gc *gc);

/* parser.c */
t_node							*create_cmd_node(char **cmd, t_gc *gc);
t_node							*create_redir_node(t_token_type type,
									t_token *red_tok, t_node *left, t_gc *gc);
t_node							*create_pipe_node(t_node *left, t_node *right,
									t_gc *gc);
int								count_cmd_words(t_token *head);
char							**build_cmd_array(t_token **head, t_env **env,
									t_gc *gc);

/* parser_build.c */
t_token							*find_last_redir(t_token *start);
t_node							*wrap_with_redir(t_token *redir, t_node *node,
									t_gc *gc);
void							skip_to_pipe(t_token **head);
t_node							*parse_simple_cmd(t_token **head, t_env **env,
									t_gc *gc);
t_node							*parse_pipeline(t_token **head, t_env **env,
									t_gc *gc);

/* main.c */
void							process_line(char *line, t_gc *gc, t_env **env);

/* builtin_utils.c */
int								ft_strcmp(const char *s1, const char *s2);
t_builtin_type					get_builtin_type(char *cmd);
void							mark_builtins(t_node *node);

/* exec.c */
int								execute_node(t_node *node, t_env **env,
									t_gc *gc);
int								execute_command(t_node *node, t_env **env,
									t_gc *gc);

/* exec_helper.c */
int								countwords(const char *s, char c);
char							*allocate(const char *s, int start, int len,
									t_gc *gc);
char							**gc_ft_split(char const *s, char c, t_gc *gc);
char							*ft_strjoin_plus(char const *s1, char const *s2,
									char const *s3, t_gc *gc);
int								env_count(t_env *env);

/* exec_utils.c */
char							*get_path(char **env, t_gc *gc);
int								check_command_type(char *cmd);
void							execute(t_node *node, t_env **env, t_gc *gc);
int								exec_cmd(t_node *node, t_env **env, t_gc *gc);

/* exec_utils2.c */
void							close_fds(int *fd);
int								execute_pipe(t_node *node, t_env **env,
									t_gc *gc);
int								execute_redir(t_node *node, t_env **env,
									t_gc *gc);

/* exec_utils3.c */
int								handle_redir_in(t_node *node, t_env **env,
									t_gc *gc);
int								handle_redir_out(t_node *node, t_env **env,
									t_gc *gc);
int								handle_redir_append(t_node *node, t_env **env,
									t_gc *gc);
int								handle_redir_heredoc(t_node *node, t_env **env,
									t_gc *gc);

/* exec_utils4.c */
char							*env_join_kv(char *key, char *value, t_gc *gc);
int								env_fill_array(char **arr, t_env *env,
									t_gc *gc);
char							**env_to_array(t_env *env, t_gc *gc);

/* segment_build.c */
t_segment						*build_segment(t_token *token, t_gc *gc);

/* segment_count.c */
int								count_segments(const char *org);

/* expansions.c */
char							*check_for_dollar(t_segment seg, t_env **env,
									t_gc *gc);
int								check_char(t_segment seg, int i);
void							initialize(t_val_full_init *val_init,
									t_segment *seg, t_gc *gc);
void							initialize2(t_val_full_init *val_init,
									t_segment *seg, t_gc *gc, t_env **env);

/* expansions_helper.c */
int								count_dollars(t_segment seg);
char							*replace_val_in_env(char *val, t_env **env,
									t_gc *gc);
int								get_lead_val_size(t_segment seg);
int								get_expandable_size(t_segment seg);

/* expansions_helper2.c */
void							init_buffers(t_val_full_init *val_init,
									t_segment *seg, t_gc *gc);
void							extract_leading_value(t_val_full_init *val_init,
									t_segment *seg);
int								is_special_char_after_dollar(char c);
void							handle_special_expan(t_val_full_init *val_init,
									t_segment *seg, t_gc *gc);
void							find_next_dollar(t_val_full_init *val_init,
									t_segment *seg);

/* expansions_helper3.c */
char							*gc_ft_itoa(int n, t_gc *gc);

/* ./builtins */
t_env							*init_env(char **envp);
char							*get_env(t_env *env, const char *key);
void							set_env(t_env **env, const char *key,
									const char *value);
void							unset_env(t_env **env, const char *key);
void							free_env(t_env *env);

int								builtin_echo(char **argv);
int								builtin_pwd(void);
int								builtin_cd(char **argv, t_env **env, t_gc *gc);
int								builtin_env(t_env *env);
int								builtin_export(t_env **env, char **args);
int								builtin_unset(t_env **env, char **args);
int								builtin_exit(char **argv, t_env **env);

int								is_valid_identifier(const char *str);
char							*ft_strndup(const char *s, size_t n);

/* builtin_export_helper.c */
t_env							*copy_env_node(t_env *node);
void							sort_env_helper(t_env **sorted,
									t_env **new_node);
t_env							*sort_env_list(t_env *env);
void							print_export(t_env *env);
void							handle_export_with_value(t_env **env,
									char *arg);

/* heredoc.c */
void							apply_redirs(t_node *n, t_env **env, t_gc *gc);
int								prepare_heredocs(t_node *n, t_env **env,
									t_gc *gc);
void							cleanup_heredocs(t_node *node);

/* heredoc_redir.c */
void							do_dup(int old, int new, const char *msg);
void							apply_heredoc(t_node *n);
void							apply_redir_in(t_node *n);
void							apply_redir_out(t_node *n);
void							apply_redir_append(t_node *n);

/* read_heredoc.c */
int								read_heredoc(const char *delim, int expand,
									t_env **env, t_gc *gc);
void							print_heredoc_warning(const char *delim);
void							write_expanded(t_segment seg, int fd,
									t_env **env, t_gc *gc);
void							write_raw(char *line, int fd);
int								check_heredoc_signal(void);

/* read_heredoc_helpers.c */
int								handle_heredoc_line(char *line,
									t_heredoc_vars *vars);
void							process_heredoc_content(char *line,
									t_heredoc_vars *here);

/* signals.c */
void							handle_sigint_interactive(int sig);
void							handle_sigint_exec(int sig);
void							handle_sigint_heredoc(int sig);
int								get_signal_exit_status(void);
void							reset_signal(void);

/* setup_signals.c */
void							setup_signals_interactive(void);
void							setup_signals_exec(void);
void							setup_signals_heredoc(void);
void							setup_signals_child(void);

/* exit_status.c */
int								get_exit_code_from_status(int status);
char							*get_exit_status(t_env *env);
void							set_exit_status(t_env **env, int status,
									char *buffer);

/* shlvl.c */
void							init_shlvl(t_env **env);

/* ft_atoi_shlvl_helpers.c */
int								skip_whitespace(const char *str);
int								parse_sign(const char *str, int *i);
int								is_overflow(long result, int sign);
long							parse_digits(const char *str, int *i, int sign);

/* int_tostr_helpers.c */
int								get_num_length(int n);
void							fill_buffer_with_digits(char *buffer, int num,
									int pos);
char							*handle_zero_case(char *buffer);

/* syntax_checks.c */
int								check_syntax(char *line);

/* syntax_helper.c */
int								check_unclosed_quotes(char *str);
void							skip_quotes(char *str, int *i);
int								handle_pipe(char *str, int *i,
									int *has_content);
int								check_pipe_helper(int *i, char *str);
int								check_pipe_syntax(char *str);

#endif
