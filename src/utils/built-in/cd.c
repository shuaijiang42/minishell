/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:49:41 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 16:43:36 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_cd(char *path)
{
	DIR			*dir;
	t_static	*s;

	s = ft_get_static();
	if (!path)
	{
		if (!ft_get_var("HOME"))
		{
			ft_err_msg("minishell: cd: HOME not set\n", NULL, NULL);
			return ;
		}
		ft_go_somewhere(s, ft_get_var("HOME"));
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
