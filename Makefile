# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/24 19:28:25 by samusanc          #+#    #+#              #
#    Updated: 2023/08/04 19:13:12 by shujiang         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
CFLAGS	= -Wall -Wextra -Werror -I ./includes/ -I ./libft/ -fsanitize=address -g3
CC		= gcc $(CFLAGS)
UTILS	= ./src/utils/
LIBFT	= -L./libft/ -lft 
LDFLAGS	= -lreadline -L /Users/$(USER)/.brew/opt/readline/lib

MAIN	= ./src/main.c 
SRCS	= $(MAIN) \
		$(UTILS)pipex/execute_cmd.c \
		$(UTILS)pipex/cmd_path.c \
		$(UTILS)pipex/error_handling.c \
		$(UTILS)pipex/main.c \
		$(UTILS)built_in.c

O_DIR	= ./objects/
OBJS	= $(addprefix $(O_DIR)/, $(SRCS:.c=.o))

$(O_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) -c $< -o $(O_DIR)/$(<:.c=.o)

all: $(NAME) $(SRCS)

$(NAME): $(OBJS)
	@make -sC ./libft/
	@$(CC) $(OBJS) $(LIBFT) $(LDFLAGS)  -o $(NAME)

re: fclean all

fclean: clean
	@make -sC ./libft/ fclean
	@rm -f $(NAME)

clean:
	@make -sC ./libft/ clean
	@rm -f $(OBJS)
	@rm -rf ./objects/

.PHONY: all
