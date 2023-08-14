/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:30:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/14 15:20:08 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXER_H
# define LEXER_H
# include <libft.h>
# include <memhandl.h>

typedef struct s_static
{
    t_list *env_cpy;
    t_list *exp;
} t_static;

typedef enum e_bool{
	false = 0,
	true = 1
}			t_bool;

typedef enum e_quotes{
	q_close = 0,
	q_open = 1
}			t_quotes;

typedef enum e_qts{
	no_q = 0,
	s_q = 1,
	d_q = 2
}			t_qts;

typedef enum e_dollar{
	not_funtional = 0,
	funtional = 1
}			t_dollar;

//el dollar sing funciona solo si las commillas son dobles
typedef struct s_command{
	t_quotes	simple_q;
	t_quotes	double_q;
	t_dollar	dollar;
	t_quotes	status;
}				t_command;

typedef struct s_cmd{
	t_qts		quotes;
	t_dollar	dollar;
	t_quotes	dollar_status;
	int			spaces;
	t_quotes	status;
}				t_cmd;

//+++++++++++++++++++++ env ++++++++++++++++++++++++++
t_static	*ft_static(int modify, t_static *new);
t_static	*ft_get_static(void);
t_static	*ft_put_static(t_static *new);
t_static	*init_struct(char **env);
//----------------------------------------------------
int			count_arguments(char *str);
int			dollar_delimiter(char c);
int			ft_lexer_check_status(t_command *cmd, char *str, int *i);
void		*ft_print_error(char *str);
int			ft_get_next_command(char *str);
int			ft_check_argument(char *str);
void		*ft_free_split_2(char ***split);
int			ft_lex_quotes(t_cmd *cmd, char c);
int			ft_lex_chars(t_cmd *cmd, char c);
int			ft_lex_space(t_cmd *cmd, char c);
int			ft_lex_chars(t_cmd *cmd, char c);
int			ft_lex_dollar(t_cmd *cmd, char c);
int			ft_lex_interrogation(t_cmd *cmd, char c);
int			ft_check_char(t_cmd *cmd, char c);
void		ft_init_cmd(t_cmd *cmd);
int			ft_dollar_len(char *str, t_cmd cmd);
void		ft_dollar_fill(char *str, t_cmd cmd, int *x, char *dst);
int			ft_lexer_len_argument(char *str);
int			ft_lexer_len_n_arguments(char *str);
void		ft_lexer_fill_str(char *str, char **str2);
int			ft_lexer_get_next_argument(char *str);
void		ft_alloc_parse_result(char ***result_ptr, char *str, int len);
char		**ft_lexer(char *str);
#endif
