# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#              #
#    Updated: 2025/12/23 20:05:36 by mohamaib         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g
LDFLAGS		= -lreadline
LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

SRCS		= main.c \
			  gc.c \
			  tokenizer.c \
			  tokenizer_words.c \
			  tokenizer_helper.c \
			  parser.c \
			  parser_build.c \
			  parser_helper.c \
			  ast_print.c \
			  token_print.c \
			  builtin_utils.c \
			  exec.c \
			  exec_utils.c \
			  exec_helper.c \
			  exec_utils2.c \
			  exec_utils3.c \
			  segment_build.c \
			  segment_count.c \
			  expansions.c \
			  expansions_helper.c \
			  heredoc.c \
			  read_heredoc.c \
			  heredoc_redir.c \
			  signals.c \
			  setup_signals.c \
			  exit_status.c \
			  shlvl.c \
			  syntax_checks.c \
			  syntax_helper.c

BUILTIN_DIR	= builtins
BUILTIN_SRCS = $(BUILTIN_DIR)/builtin_cd.c \
			   $(BUILTIN_DIR)/builtin_echo.c \
			   $(BUILTIN_DIR)/builtin_env.c \
			   $(BUILTIN_DIR)/builtin_exit.c \
			   $(BUILTIN_DIR)/builtin_export.c \
			   $(BUILTIN_DIR)/builtin_export_helper.c \
			   $(BUILTIN_DIR)/builtin_pwd.c \
			   $(BUILTIN_DIR)/builtin_unset.c \
			   $(BUILTIN_DIR)/env_getset.c \
			   $(BUILTIN_DIR)/env_init.c \
			   $(BUILTIN_DIR)/env_utils.c

ALL_SRCS	= $(SRCS) $(BUILTIN_SRCS)
OBJS		= $(ALL_SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
