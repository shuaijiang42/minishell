/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utiles.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:44:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/05 13:55:45 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_get_var(char *var)
{
	t_static	*s;
	char		*env_var;
	size_t		len;
	t_list		*temp;

	s = ft_get_static();
	len = ft_strlen(var);
	temp = s->env;
	while (temp)
	{
		env_var = temp->content;
		if (env_var && var && !ft_strncmp(env_var, var, len)
			&& env_var[ft_strlen(var)] == '=')
			return (env_var + len + 1);
		temp = temp->next;
	}
	return (NULL);
}

void	update_dir_for_exp(t_static *s)
{
	t_list	*oldpwd;
	t_list	*old;
	t_list	*new;
	t_list	*new_oldpwd;
	char	buf[4096];

	old = NULL;
	new = ft_lstnew(ft_strjoin("declare -x PWD=", getcwd(buf, sizeof(buf))));
	old = ft_locate_node(s->exp, "declare -x PWD");
	new_oldpwd = ft_lstnew(ft_strjoin("declare -x OLD", old->content + 11));
	oldpwd = ft_locate_node(s->exp, "declare -x OLDPWD");
	ft_node_substitute(&oldpwd, &new_oldpwd);
	ft_node_substitute(&old, &new);
}

void	update_dir_for_env(t_static *s)
{
	t_list	*oldpwd;
	t_list	*old;
	t_list	*new;
	t_list	*new_oldpwd;
	char	buf[4096];

	old = NULL;
	oldpwd = NULL;
	new = ft_lstnew(ft_strjoin("PWD=", getcwd(buf, sizeof(buf))));
	old = ft_locate_node(s->env, "PWD");
	if (old)
		new_oldpwd = ft_lstnew(ft_strjoin("OLD", old->content));
	oldpwd = ft_locate_node(s->env, "OLDPWD");
	ft_node_substitute(&oldpwd, &new_oldpwd);
	ft_node_substitute(&old, &new);  
}

void	ft_go_somewhere(t_static *s, char *somewhere)
{
	chdir(somewhere);
	update_dir_for_env(s);
	update_dir_for_exp(s);
}

void	ft_err_msg(char *s1, char *path, char *s2)
{
	ft_putstr_fd(s1, STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(s2, STDERR_FILENO);
	errno = 1;
	ft_put_error(1);
}
