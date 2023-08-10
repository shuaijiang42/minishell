/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:21:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/10 20:03:24 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
# define MINISHELL_H

# include <ft_lexer.h>
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

typedef struct s_global
{
    t_list *env_cpy;
    t_list *exp;
} t_global;

t_global *g;


void	rl_replace_line(const char *text, int clear_undo);
void	execve_with_error_check(char **argv, char **env);
int     ft_built_in(char **input);
void	ft_free_input(char **input);
void    ft_excuter(char **input, char **env);
void    env_copy(char **env);
void    print_env_cpy(void);
void    ft_env(char **input);
void    ft_unset(char **input);
void    init_global(char **env);
void	creat_exp_list(char **env);
void    print_exp(void);
void	ft_export(char **input);

#endif
