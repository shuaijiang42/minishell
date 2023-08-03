/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:21:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/03 13:58:20 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
# define MINISHELL_H

# include <libft.h>
# include <pipex.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

/* Replace_line shenanigans */
void			rl_replace_line(const char *text, int clear_undo);

void	execve_with_error_check(char **argv, char **env);
void    pipex(char *input, char **env);
#endif
