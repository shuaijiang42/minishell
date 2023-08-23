/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:09:21 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/23 17:13:24 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**split_path(char **env)
{
	char	*all_path;
	char	**path;
	int		i;

	i = 0;
	all_path = NULL;
	while (env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
			all_path = ft_strdup(env[i] + 5);
		i++;
	}
	if (all_path == NULL)
		return (NULL);
	path = ft_split(all_path, ':');
	free(all_path);
	return (path);
}

void	*ft_free_split(void **strings)
{
	int	i;

	i = 0;
	while (strings && strings[i])
	{
		free(strings[i]);
		i++;
	}
	free(strings);
	return (NULL);
}

//get cmd, remove flags
char	*get_cmd(char *argv)
{
	/*
	char	**temp;
	char	*cmd;

	temp = ft_split(argv, ' ');
	if (*temp == NULL)
		print_error(" : command not found");
	cmd = ft_strdup(temp[0]);
	ft_free_split((void **)temp);
	*/
	char *cmd;
	cmd = ft_strdup(argv);
	return (cmd);
}

char	*get_full_path(char **env, char *cmd)
{
	char	**path_list;
	int		i;
	char	*path;
	char	*temp;

	i = 0;
	path = NULL;
	temp = NULL;
	path_list = split_path(env);
	while (path_list && path_list[i])
	{
		temp = ft_strjoin(path_list[i], "/");
		path = ft_strjoin(temp, cmd);
		if (access(path, X_OK) == 0)
		{
			free(temp);
			if (path_list)
				ft_free_split_2((char ***)&path_list);
			return (path);
		}
		ft_free((void **)&temp);
		ft_free((void **)&path);
		i++;
	}
	//printf("ptr:%p\n", path_list);
	if (path_list)
		ft_free_split_2((char ***)&path_list);
	exit_cmd_not_found(path, temp, cmd);
	return (0);
}

char	*cmd_path(char *argv, char **env)
{
	char	*cmd;

	cmd = get_cmd(argv);
	if (ft_strncmp("/", cmd, 1) == 0 || ft_strncmp("./", cmd, 2) == 0
		|| ft_strncmp("../", cmd, 3) == 0)
		return (cmd);
	return (get_full_path(env, cmd));
}
