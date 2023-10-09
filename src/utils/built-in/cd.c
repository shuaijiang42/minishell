/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:49:41 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/09 12:18:42 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	go_to_home(void)
{
	t_list	*home;
	char	*str;

	home = search_node_env("HOME=", (ft_get_static())->env);
	if (!home)
	{
		ft_err_msg("minishell: cd: HOME not set\n", NULL, NULL);
		return ;
	}
	str = get_var_from_node(home);
	ft_go_somewhere(ft_get_static(), str);
	ft_free((void **)&str);
}

void	ft_cd(char *path)
{
	DIR			*dir;
	t_static	*s;

	s = ft_get_static();
	if (!path)
	{
		go_to_home();
		return ;
	}
	dir = opendir(path);
	if (dir)
	{
		if (access(path, X_OK) == -1)
			ft_err_msg("minishell: cd: ", path, ": Permission denied\n");
		else
			ft_go_somewhere(s, path);
		closedir(dir);
	}
	else
		ft_err_msg("minishell: cd: ", path, "No such file or directory\n");
}
