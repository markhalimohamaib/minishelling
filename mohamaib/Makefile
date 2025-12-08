# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mohamaib <mohamaib@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/05 00:00:00 by mohamaib          #+#    #+#              #
#    Updated: 2025/12/05 22:03:34 by mohamaib         ###   ########.fr        #
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
			  parser.c \
			  parser_build.c \
			  ast_print.c \
			  token_print.c \
			  builtin_utils.c \
			  exec.c \
			  exec_utils.c \
			  exec_utils2.c \
			  exec_utils3.c \
			  segment_build.c \
			  expansions.c

BUILTIN_DIR	= builtins
BUILTIN_SRCS = $(BUILTIN_DIR)/builtin_cd.c \
			   $(BUILTIN_DIR)/builtin_echo.c \
			   $(BUILTIN_DIR)/builtin_env.c \
			   $(BUILTIN_DIR)/builtin_exit.c \
			   $(BUILTIN_DIR)/builtin_export.c \
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
