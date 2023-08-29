/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_global.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:30:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/29 16:10:24 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FT_GLOBAL_H
# define FT_GLOBAL_H
# include <libft.h>


typedef struct s_static
{
    t_list	*env;
    t_list	*exp;
	t_list	*error;
	t_list	*history;
    char    *pwd;
    char    *oldpwd; 
    int     shlvl;
    char    *last_cmd;
} t_static;

//+++++++++++++++++++++ env ++++++++++++++++++++++++++
t_static	*ft_static(int modify, t_static *new);
t_static	*ft_get_static(void);
t_static	*ft_put_static(t_static *new);
t_static	*init_struct(char **env);
int			ft_get_error(void);
void		ft_put_error(int error);
int			ft_get_history(void);
void		ft_put_history(int n);
int			ft_get_proccess(void);
void		ft_put_proccess(int n);
//----------------------------------------------------

#endif
