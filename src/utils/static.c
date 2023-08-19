/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 13:08:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/19 17:40:15 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_static *init_struct(char **env)
{
	t_static *s;
	int			*i;
	
	s = NULL;
    s = calloc(1, sizeof(*s));
    if(!s)
	{
        perror("calloc: ");
		return (NULL);
	}
    env_copy(env, s);
    creat_exp_list(env, s);
	i = malloc(sizeof(int));
	if (!i)
	{
        perror("malloc: ");
		return (NULL);
	}
	*i = 0;
	s->error = ft_lstnew((void *)i);
	return (s);
}

int	ft_get_error(void)
{
	return (*(int *)(ft_get_static())->error->content);
}

void	ft_put_error(int error)
{
	*(int *)(ft_get_static())->error->content = error;
}

t_static *ft_static(int modify, t_static *new)
{
	static t_static *s = NULL;
	if (modify == 1)
		s = new;
	return (s);
}

t_static	*ft_get_static(void)
{
	return (ft_static(0, NULL));
}

t_static	*ft_put_static(t_static *new)
{
	return (ft_static(1, new));
}
