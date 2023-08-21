/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:30:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/21 15:14:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef STRUCT_UTILS_H
# define STRUCT_UTILS_H

typedef	struct s_pipes{
	int	start_pipe[2];
	int	fd;
	int	end_pipe[2];
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

#endif
