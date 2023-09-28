/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_static_struct.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:34:53 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:19:22 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_get_info_from_env(char **env, char *var_name)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	if (var_name)
		len = ft_strlen(var_name);
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, len) == 0)
			return (env[i]);
		i++;
	}
	return (NULL);
}

void	ft_update_shlvl(char **env)
{
	int		i;
	char	*shlvl;
	int		nb;
	char	*new_shlvl;

	i = 0;
	new_shlvl = NULL;
	nb = 0;
	shlvl = NULL;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "SHLVL=", 6))
		{
			shlvl = ft_substr(env[i], 6, ft_strlen(env[i]));
			nb = ft_atoi(shlvl);
			free(shlvl);
			nb++;
			new_shlvl = ft_itoa(nb);
			env[i] = ft_strjoin("SHLVL=", new_shlvl);
			free(new_shlvl);
			break ;
		}
		i++;
	}
}

t_static	*ft_setup_struct(char **env, t_static *s)
{
	if (s->here == -1)
	{
		perror("dup: ");
		return (NULL);
	}
	if (!(*env))
	{
		s->oldpwd = ft_lstnew("OLDPWD");
		s->last_cmd = ft_lstnew("_=./minishell");
	}
	else
	{
		ft_update_shlvl(env);
		s->oldpwd = ft_lstnew(ft_get_info_from_env(env, "OLDPWD="));
		s->last_cmd = ft_lstnew(ft_get_info_from_env(env, "_="));
	}
	return (s);
}

t_static	*init_static_struct(char **env)
{
	t_static	*s;
	int			*i;
	char		pwd[4096];

	s = calloc(1, sizeof(t_static));
	if (!s)
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
	s->here = dup(STDIN_FILENO);
	return (ft_setup_struct(env, s));
}
