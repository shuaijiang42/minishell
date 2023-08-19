/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:30:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/19 17:27:43 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_GLOBAL_H
# define FT_GLOBAL_H
# include <libft.h>
typedef struct s_static
{
    t_list	*env_cpy;
    t_list	*exp;
	t_list	*error;
} t_static;

//+++++++++++++++++++++ env ++++++++++++++++++++++++++
t_static	*ft_static(int modify, t_static *new);
t_static	*ft_get_static(void);
t_static	*ft_put_static(t_static *new);
t_static	*init_struct(char **env);
int			ft_get_error(void);
void		ft_put_error(int error);
//----------------------------------------------------
#endif
