/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 13:08:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/30 12:48:19 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_get_old_history(char **env, int *fd)
{
	int		i;
	char	*str;
	char	*str2;
	char	*str3;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		if (!ft_strncmp("HOME=", env[i], 5))
			break ;
		i++;
	}
	if (!env[i])
		return ;
	str = *(env + i);
	str += 5;
	str2 = ft_strjoin(str, "/.minishell_history");
	*fd = open(str2, O_CREAT | O_RDWR | O_APPEND, 0644);
	ft_free((void *)&str2);
	if (*fd < 0)
		return ;
	str3 = get_next_line(*fd);
	while(str3)
	{
		str3[ft_strlen(str3) - 1] = '\0';
		if (str3)
			add_history(str3);
		ft_free((void *)&str3);
		str3 = get_next_line(*fd);
	}
}

int	ft_static_history(int fd_n, int i)
{
	static int	fd = 0;

	if (!i)
		fd = fd_n;
	return (fd);
}

int	ft_get_history(void)
{
	return(ft_static_history(0, 1));
}

void	ft_put_history(int n)
{
	ft_static_history(n, 0);
}

int	ft_static_proccess(int fd_n, int i)
{
	static int	fd = 0;

	if (!i)
		fd = fd_n;
	return (fd);
}

int	ft_get_proccess(void)
{
	return(ft_static_history(0, 1));
}

void	ft_put_proccess(int n)
{
	ft_static_history(n, 0);
}

/* t_static *init_struct(char **env)
{
	t_static *s;
	int			*i;
	char buf[4096];
	const char *pwd;
	char *shlvl;

	
	s = NULL;
    s = calloc(1, sizeof(*s));
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
	s->history = NULL;
	pwd = getcwd(buf, sizeof(buf));
	s->pwd = ft_strdup(pwd);
	s->shlvl = 1;
	shlvl = ft_strjoin("SHLVL=", ft_itoa(s->shlvl));
	
	if (!env)
	{
		s->env = ft_lstnew(s->pwd);
		ft_lstadd_back(&(s->env), ft_lstnew(shlvl));
		ft_lstadd_back(&(s->env), ft_lstnew("_=./minishell"));
		creat_exp_list(s);
		return (s);
	}
    ft_copy_env(env);
    creat_exp_list(s);
	return (s);
} */

t_static *init_struct(char **env)
{
	t_static *s;
	int			*i;
	char buf[4096];

	
	s = NULL;
    s = calloc(1, sizeof(*s));
    if(!s)
	{
        perror("calloc: ");
		return (NULL);
	}
	const char *pwd = getcwd(buf, sizeof(buf));
	s->pwd = ft_strdup(pwd);
    ft_copy_env(env);
    creat_exp_list(s);
	i = malloc(sizeof(int));
	if (!i)
	{
        perror("malloc: ");
		return (NULL);
	}
	*i = 0;
	s->error = ft_lstnew((void *)i);
	s->history = NULL;
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
