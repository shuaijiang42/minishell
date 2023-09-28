# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/24 19:28:25 by samusanc          #+#    #+#              #
#    Updated: 2023/09/28 12:51:14 by shujiang         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#	GENERAL FLAGS
NAME	= minishell
CFLAGS	= -Wall -Wextra -Werror -I ./includes/ -I ./libft/ -fsanitize=address -g3
CC		= gcc $(CFLAGS)
UTILS	= ./src/utils/
#	UTILS VARIABLES
#		LEXER
LEX		= $(UTILS)lexer/
LEX_U	= $(LEX)utils/
#		EXECUTER AND PROCCESS MAKER
EXC		= $(UTILS)execution/executer/
EXC_U	= $(EXC)utils/
PRCS	= $(UTILS)execution/proccess/
PRCS_U	= $(PRCS)utils/
#		BUILTIN
BUILT_IN= $(UTILS)built-in/
#		STATIC
STATIC  = $(UTILS)static/
#		SIGNALS
SIGNAL  = $(UTILS)signal/
#	LIBFT AND READLINE
LIBFT	= -L./libft/ -lft 
LDFLAGS	= -lreadline -L /Users/$(USER)/.brew/opt/readline/lib -fsanitize=address -g3

MAIN	= ./src/main.c 
SRCS	= $(MAIN) \
		$(UTILS)pipex/execute_cmd.c \
		$(UTILS)pipex/cmd_path.c \
		$(UTILS)pipex/error_handling.c \
		$(UTILS)pipex/main.c \
		$(BUILT_IN)built_in.c \
		$(BUILT_IN)built_in_utiles.c \
		$(BUILT_IN)env.c \
		$(BUILT_IN)echo.c \
		$(BUILT_IN)exit.c \
		$(BUILT_IN)pwd.c \
		$(BUILT_IN)cd.c \
		$(BUILT_IN)cd_utiles.c \
		$(BUILT_IN)export.c \
		$(BUILT_IN)export_utiles.c \
		$(BUILT_IN)export_utiles2.c \
		$(BUILT_IN)list.c \
		$(BUILT_IN)list_utiles.c \
		$(BUILT_IN)unset_utiles.c \
		$(BUILT_IN)unset.c \
		$(STATIC)static.c \
		$(STATIC)init_static_struct.c \
		$(SIGNAL)signal.c \
		\
		$(PRCS)proccess.c				$(PRCS_U)count_pipes.c \
		$(PRCS_U)first_child.c			$(PRCS_U)ft_strndup.c			$(PRCS_U)get_cmd_pipex.c \
		$(PRCS_U)get_next_index.c		$(PRCS_U)last_child.c			$(PRCS_U)mid_child.c \
		$(PRCS_U)pipex.c				$(PRCS_U)test_input.c			$(PRCS_U)wait_all_childs.c \
		 \
		$(EXC)executer.c \
		$(EXC_U)cut_input.c				$(EXC_U)exc_chang_in.c \
		$(EXC_U)exc_change_out_apd.c	$(EXC_U)exc_change_out_trc.c	$(EXC_U)exc_clear_content.c \
		$(EXC_U)exc_free_content.c		$(EXC_U)exc_lex_word.c			$(EXC_U)exc_make_cmd.c \
		$(EXC_U)exc_make_redir.c		$(EXC_U)exc_make_word.c			$(EXC_U)exc_new_node.c \
		$(EXC_U)exc_open_fd.c			$(EXC_U)executer_exec.c			$(EXC_U)ft_check_char.c \
		$(EXC_U)ft_check_dup_redir.c	$(EXC_U)ft_check_redirs.c		$(EXC_U)ft_error_exc.c \
		$(EXC_U)ft_exc_execution.c		$(EXC_U)ft_init_exc.c			$(EXC_U)ft_status_redir.c \
		$(EXC_U)ft_strlen2.c			$(EXC_U)get_redir.c				$(EXC_U)good_strjoin.c \
		$(EXC_U)here_doc.c				$(EXC_U)lex_input.c				$(EXC_U)make_list.c \
		 \
		$(UTILS)Samu_GNL/get_next_line.c \
		$(UTILS)Samu_GNL/get_next_line_utils.c \
		$(UTILS)history/ft_history.c \
		$(UTILS)history/save_history.c \
		 \
		$(LEX)holaaaa.c \
		$(LEX_U)utils.c					$(LEX_U)count_arguments.c		$(LEX_U)dollar_delimiter.c \
		$(LEX_U)ft_lexer_check_status.c	$(LEX_U)ft_get_next_command.c	$(LEX_U)ft_check_argument.c \
		$(LEX_U)ft_free_split2.c		$(LEX_U)ft_lex_quotes.c			$(LEX_U)ft_lex_delimiters.c \
		$(LEX_U)ft_lex_space.c			$(LEX_U)ft_lex_chars.c			$(LEX_U)ft_lex_dollar.c \
		$(LEX_U)ft_lex_interrogation.c	$(LEX_U)ft_check_char.c			$(LEX_U)ft_init_cmd.c \
		$(LEX_U)ft_dollar_len.c			$(LEX_U)ft_dollar_fill.c		$(LEX_U)ft_lexer_utils.c \
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
