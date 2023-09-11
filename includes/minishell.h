/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:21:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/11 18:54:25 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
# define MINISHELL_H

# include <struct_utils.h>
# include <get_next_line_samu.h>
# include <ft_global.h>
# include <memhandl.h>
# include <lexer.h>
# include <libft.h>
# include <sys/errno.h>
# include <pipex.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <ft_signals.h>

int flag;
void	rl_replace_line(const char *text, int clear_undo);
int		execve_with_error_check(char **argv, char **env);
int     ft_built_in(char **input);
void	ft_free_input(char **input);
int    ft_excuter(char **input, char **env);
//void env_copy(char **env, t_static *s);
void    print_env_cpy(void);
void    ft_env(char **input);
void    ft_unset(char **input);
int	ft_parsing(char	*str);
void		 print_exp(void);
void		ft_export(char **input);
t_static 	*ft_static(int modify, t_static *new);
t_static	*ft_get_static(void);
t_static	*ft_put_static(t_static *new);

/* void	creat_exp_list(char **env, t_static *s); */
void	creat_exp_list(t_static *s);
void    handler(int signal);
void    ft_remove_node(t_list *list, int index);


t_static *init_static_struct();

int			ft_exc_make_redir(char *cmd, t_input *line);
int			executer(char *cmd, t_input *input);
void		ft_procces_maker(char *cmd, char **env);
void		ft_get_old_history(char **env, int *fd);
char        *get_var_info(char *var_name);

void    ft_copy_env(char **env);
void    add_list_and_sort(t_list **list, t_list *new);
t_list  *ft_locate_node(t_list *list, char *var_name);
void    ft_node_substitute(t_list **old, t_list **new);
char    **list_to_matrix(t_list *list);

#endif
