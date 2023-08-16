/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:21:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/15 18:20:45 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
# define MINISHELL_H

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

void	rl_replace_line(const char *text, int clear_undo);
void	execve_with_error_check(char **argv, char **env);
int     ft_built_in(char **input);
void	ft_free_input(char **input);
void    ft_excuter(char **input, char **env);
void env_copy(char **env, t_static *s);
void    print_env_cpy(void);
void    ft_env(char **input);
void    ft_unset(char **input);
int	ft_parsing(char	*str);
void    print_exp(void);
void	ft_export(char **input);
t_static *ft_static(int modify, t_static *new);
t_static	*ft_get_static(void);
t_static	*ft_put_static(t_static *new);
t_static *init_struct(char **env);
void	creat_exp_list(char **env, t_static *s);
#endif
