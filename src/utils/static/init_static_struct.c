/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_static_struct.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:34:53 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/08 18:29:06 by samusanc         ###   ########.fr       */
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

void	change_content_env(t_list *list, char *content)
{
	if (!list)
		return ;
	ft_free((void **)&list->content);
	list->content = (void *)content;
}

//if is the variable return 1, else return 0
size_t	compare_var(char *var, char *node)
{
	char	*str;

	str = NULL;
	if (!var || !node)
		return (0);
	if (!ft_strlen(var) || !ft_strlen(node))
		return (0);
	if (!ft_strncmp(var, node, ft_strlen(var)))
		return (1);
	if (ft_strlen(var))
	{
		if (var[ft_strlen(var) - 1] == '=')
		{
			str = ft_strdup(var);
			str[ft_strlen(var) - 1] = '\0';
			if (!ft_strncmp(var, node, ft_strlen(node)))
				return (1);
		}
	}
	ft_free((void **)&str);
	return (0);
}

//search one node if not found return NULL
t_list	*search_node_env(char *str, t_list *env)
{
	size_t	i;
	t_list	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		if (tmp->content && str)
		{
		//	printf("this is the compare:%s, %s.\nresult:%d\n", tmp->content, str, compare_var(str, tmp->content));
			if (compare_var(str, tmp->content))
				break ;
		}
		tmp = tmp->next;
	}
	return (tmp);
}

//get var from node and malloc the str
char	*get_var_from_node(t_list *node)
{
	size_t	i;
	char	*content;

	i = 0;
	content = (char *)node->content;
	if (!node || !node->content)
		return (NULL);
	while (content[i] && content[i] != '=')
		i++;
	if (content[i])
		i++;
	return (ft_strdup(content + i));
}

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
	ft_lstadd_back(&s->env, ft_lstnew(ft_strjoin("PWD=", getcwd(buf, sizeof(buf)))));
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

t_static	*ft_setup_struct(char **env, t_static *s)
{

	if (!(*env))
		init_default_env(s);
	else
		copy_external_env(s, env);
	shlvl_sum(s);
	return (s);
}

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
	char		pwd[4096];

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
	return (ft_setup_struct(env, s));
}
