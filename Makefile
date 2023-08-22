# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/24 19:28:25 by samusanc          #+#    #+#              #
#    Updated: 2023/08/22 16:59:09 by samusanc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
CFLAGS	= -Wall -Wextra -Werror -I ./includes/ -I ./libft/ -fsanitize=address -g3
CC		= gcc $(CFLAGS)
UTILS	= ./src/utils/
LEX		= $(UTILS)lexer/
LIBFT	= -L./libft/ -lft 
LDFLAGS	= -lreadline -L /Users/$(USER)/.brew/opt/readline/lib

MAIN	= ./src/main.c 
SRCS	= $(MAIN) \
		$(UTILS)pipex/execute_cmd.c \
		$(UTILS)pipex/cmd_path.c \
		$(UTILS)pipex/error_handling.c \
		$(UTILS)pipex/main.c \
		$(UTILS)built_in.c \
		$(UTILS)unset.c \
		$(UTILS)env.c \
		$(UTILS)export.c \
		$(UTILS)static.c \
		$(LEX)holaaaa.c \
		$(UTILS)execution/proccess.c \
		$(UTILS)execution/executer.c \
		#$(LEX)utils/ft_lexer_utils.c \
		$(LEX)checker/check_input.c \
		$(LEX)checker/ft_lexer_check_status.c \
		$(LEX)checker/ft_get_next_command.c \

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
