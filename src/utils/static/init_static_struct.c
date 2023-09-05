/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_static_struct.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:34:53 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/04 18:07:18 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_static *init_static_struct(void)
{
    t_static *s;
    int *i;
    char pwd[4096];
    
    s = calloc(1, sizeof(t_static));
    if(!s)
	{
        perror("calloc: ");
		return (NULL);
	}
    i = malloc(sizeof(int));
	if (!i)
	{
        perror("malloc: ");
		return (NULL);
	}
	*i = 0;
	s->error = ft_lstnew((void *)i);
    s->pwd = ft_lstnew((void *)getcwd(pwd, sizeof(pwd)));
    s->oldpwd = ft_lstnew(NULL);
    s->last_cmd = ft_lstnew((void *)"./minishell");
    return (s);
}
   
   