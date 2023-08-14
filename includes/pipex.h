/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:30:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/14 10:55:05 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <libft.h>
# include <sys/wait.h>
# include <lexer.h>

# ifndef READ
#  define READ 0
# endif
# ifndef WRITE
#  define WRITE 1
# endif

typedef struct s_var5
{
	int		argc;
	char	**argv;
	char	**env;
	int		i;
	int		fd;
}	t_var5;

typedef struct s_var2
{
	int		i;
	int		fd;
}	t_var2;

char	*cmd_path(char *argv, char **env);
int		execute_cmd(t_var5 var);
void	pipe_with_error_check(int *fd);
void	dup2_with_error_check(int fd1, int fd2);
void	close_with_error_check(int fd);
int		open_with_error_check(const char *path, int oflag);
void	execve_with_error_check(char **argv, char **env);
int		fork_with_error_check(void);
void	exit_cmd_not_found(char *path, char *temp, char *cmd);
void	print_error(char *str);

#endif
