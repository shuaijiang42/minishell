# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/24 19:28:25 by samusanc          #+#    #+#              #
#    Updated: 2023/09/20 18:56:02 by samusanc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell
CFLAGS	= -Wall -Wextra -Werror -I ./includes/ -I ./libft/ -fsanitize=address -g3
CC		= gcc $(CFLAGS)
UTILS	= ./src/utils/
LEX		= $(UTILS)lexer/
LEX_U	= $(LEX)utils/
BUILT_IN= $(UTILS)built-in/
STATIC  = $(UTILS)static/
SIGNAL  = $(UTILS)signal/
LIBFT	= -L./libft/ -lft 
LDFLAGS	= -lreadline -L /Users/$(USER)/.brew/opt/readline/lib -fsanitize=address -g3

MAIN	= ./src/main.c 
SRCS	= $(MAIN) \
		$(UTILS)pipex/execute_cmd.c \
		$(UTILS)pipex/cmd_path.c \
		$(UTILS)pipex/error_handling.c \
		$(UTILS)pipex/main.c \
		$(BUILT_IN)built_in.c \
		$(BUILT_IN)unset.c \
		$(BUILT_IN)env.c \
		$(BUILT_IN)export.c \
		$(BUILT_IN)list.c \
		$(STATIC)static.c \
		$(STATIC)init_static_struct.c \
		$(SIGNAL)signal.c \
		$(UTILS)execution/proccess.c \
		$(UTILS)execution/executer.c \
		$(UTILS)Samu_GNL/get_next_line.c \
		$(UTILS)Samu_GNL/get_next_line_utils.c \
		$(UTILS)history/ft_history.c \
		$(LEX)holaaaa.c \
		$(LEX_U)utils.c $(LEX_U)count_arguments.c $(LEX_U)dollar_delimiter.c \
		$(LEX_U)ft_lexer_check_status.c $(LEX_U)ft_get_next_command.c $(LEX_U)ft_check_argument.c \
		$(LEX_U)ft_free_split2.c $(LEX_U)ft_lex_quotes.c $(LEX_U)ft_lex_delimiters.c \
		$(LEX_U)ft_lex_space.c $(LEX_U)ft_lex_chars.c $(LEX_U)ft_lex_dollar.c \
		$(LEX_U)ft_lex_interrogation.c $(LEX_U)ft_check_char.c $(LEX_U)ft_init_cmd.c \
		$(LEX_U)ft_dollar_len.c $(LEX_U)ft_dollar_fill.c $(LEX_U)ft_lexer_utils.c \
		$(LEX_U)ft_lexer_len.c \
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
	@make bonus -sC ./libft/
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
