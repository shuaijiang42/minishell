/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utiles.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:44:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/08 19:06:50 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_list	*get_or_add_pwd(char *str_pwd, t_list *env, char *content)
{
	t_list	*pwd;

	pwd = search_node_env(str_pwd, env);
	if (!pwd)
	{
		ft_lstadd_back(&env, ft_lstnew(ft_good_strjoin(str_pwd, content)));
		pwd = search_node_env(str_pwd, env);
	}
	return (pwd);
}

void	update_dir_for_env(t_static *s)
{
	char	buf[4096];
	t_list	*pwd;
	t_list	*oldpwd;
	t_list	*env;
	char	*tmp;

	if (!getcwd(buf, sizeof(buf)))
	{
		perror("GETCWD: ");
		exit(-1);
	}
	env = s->env;
	pwd = get_or_add_pwd("PWD=", env, (char *)buf);
	oldpwd = get_or_add_pwd("OLDPWD=", env, pwd->content);
	tmp = get_var_from_node(pwd);
	change_content_env(oldpwd, ft_good_strjoin("OLDPWD=", tmp));
	change_content_env(pwd, ft_good_strjoin("PWD=", buf));
	ft_free((void **)&tmp);
}

void	ft_go_somewhere(t_static *s, char *somewhere)
{
	chdir(somewhere);
	update_dir_for_env(s);
}

void	ft_err_msg(char *s1, char *path, char *s2)
{
	ft_putstr_fd(s1, STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(s2, STDERR_FILENO);
	errno = 1;
	ft_put_error(1);
}
