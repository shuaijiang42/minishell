/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_static_struct.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:34:53 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/09 12:27:12 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*error_init_static(int x, int *i, t_static *s)
{
	if (s->here)
		close(s->here);
	ft_free((void **)&i);
	if (s)
		ft_free((void **)&s->error);
	ft_free((void **)&s);
	if (x == 1)
		perror("malloc: ");
	else
		perror("dup: ");
	return (NULL);
}

t_static	*init_static_struct(char **env)
{
	t_static	*s;
	int			*i;

	i = NULL;
	s = ft_calloc(1, sizeof(t_static));
	if (!s)
		return (error_init_static(1, i, s));
	i = malloc(sizeof(int));
	if (!i)
		return (error_init_static(1, i, s));
	*i = 0;
	s->here = 0;
	s->here = dup(STDIN_FILENO);
	if (s->here == -1)
		return (error_init_static(2, i, s));
	s->error = NULL;
	s->error = ft_lstnew((void *)i);
	if (!s->error)
		return (error_init_static(1, i, s));
	return (ft_setup_env(env, s));
}
