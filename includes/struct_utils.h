/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:30:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/30 18:45:12 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef STRUCT_UTILS_H
# define STRUCT_UTILS_H
# include <lexer.h>
# define ACTIVE 1
# define USED 2
# define INACTIVE 0

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

#endif
