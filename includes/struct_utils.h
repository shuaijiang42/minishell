/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:30:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/20 19:00:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef STRUCT_UTILS_H
# define STRUCT_UTILS_H
# include <lexer.h>
# define ACTIVE 1
# define USED 2
# define INACTIVE 0
# include <limits.h>

typedef enum e_redir{
	non	= 0,
	inp = 1,
	hre = 2,
	trc = 3,
	apd = 4
}			t_redir;

typedef	struct s_pipes{
	int	start_pipe[2];
}				t_pipes;

typedef	struct s_pipstr{
	char	*cmd_cpy;
	int		n;
	int		i;
	t_pipes	pipes;
	int		*tmp_pipe;
	int		status;
	char	*cmd;
}				t_pipstr;

typedef	struct s_argument{
	char	*str;
	char	*type;
	int		fd;
}				t_argument;

typedef struct s_exc_lex{
	int		i;
	int		j;
	int		major;
	int		minor;
	int		redirs;
	t_redir	status;
	int		word;
	int		space;
	int		in;
	int		out;
	char	first;
	char	*input;
	t_cmd	cmd;
}				t_exc_lex;

typedef struct s_dollar_fill{
	int		j;
	int		i;
	int		z;
	char	*str2;
	char 	*str3;
	t_list	*tmp;
	//	external
	char	*dst;
	int		*x;
}				t_dollar_fill;

typedef struct s_input{
	char	*cmd;
	int		in;
	int		out;
	int		here;
}				t_input;

typedef struct s_dollar_len{
	int		i;
	int		j;
	int		return_value;
	char	*str;
	char	*str2;
	t_list	*tmp;
	t_cmd	cmd;
}				t_dollar_len;

typedef struct s_get_next_command{
	int			i;
	int			command;
	int			n_commands;
	int			end;
	int			return_status;
	char		*str;
	t_command	status;
}				t_get_next_command;

typedef struct s_save_history{
	t_list	*history;
	t_list	*old_history;
	int		fd;
	char	**env;
	int		len;
}				t_save_history;

#endif
