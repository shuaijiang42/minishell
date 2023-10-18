/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_static_struct_utils2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 12:28:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/09 12:29:19 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	shlvl_err(t_list *shlvl, char *str1)
{
	change_content_env(shlvl, ft_strdup("SHLVL=0"));
	ft_free((void **)&str1);
	return (0);
}

int	shlvl_sum(t_static *s)
{
	t_list	*shlvl;
	int		i;
	char	*str1;
	char	*str2;

	i = 0;
	shlvl = search_node_env("SHLVL", s->env);
	if (!shlvl)
		ft_lstadd_back(&s->env, ft_lstnew("SHLVL=0"));
	shlvl = search_node_env("SHLVL", s->env);
	str1 = get_var_from_node(shlvl);
	i = ft_atoi(str1);
	if (i < 0)
		return (shlvl_err(shlvl, str1));
	i += 1;
	str2 = ft_itoa(i);
	if (str2)
		change_content_env(shlvl, ft_strjoin("SHLVL=", str2));
	else
		change_content_env(shlvl, ft_strjoin("SHLVL=", "0"));
	ft_free((void **)&str1);
	ft_free((void **)&str2);
	return (0);
}

void	init_default_env(t_static *s)
{
	char	buf[4096];

	s->env = ft_lstnew(ft_strdup("OLDPWD"));
	ft_lstadd_back(&s->env, \
	ft_lstnew(ft_strjoin("PWD=", getcwd(buf, sizeof(buf)))));
	ft_lstadd_back(&s->env, ft_lstnew(ft_strdup("SHLVL=0")));
	ft_lstadd_back(&s->env, ft_lstnew(ft_strdup("_=./minishell")));
}

void	copy_external_env(t_static	*s, char **env)
{
	size_t	i;

	i = 0;
	while (env[i])
		ft_lstadd_back(&s->env, ft_lstnew(ft_strdup(env[i++])));
}

t_static	*ft_setup_env(char **env, t_static *s)
{
	if (!(*env))
		init_default_env(s);
	else
		copy_external_env(s, env);
	shlvl_sum(s);
	return (s);
}
