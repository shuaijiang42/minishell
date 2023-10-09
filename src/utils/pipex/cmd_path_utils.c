/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 12:44:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/09 12:49:49 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

char	*get_cmd(char *argv)
{
	char	*cmd;

	cmd = ft_strdup(argv);
	return (cmd);
}

char	*get_full_path_util(char **path_list, char *path, char *temp, char *cmd)
{
	char	*script;

	if (path_list)
	{
		ft_free_split_2((char ***)&path_list);
		exit_cmd_not_found(path, temp, cmd);
		return (NULL);
	}
	else
	{
		script = ft_strjoin("./", cmd);
		free(cmd);
		return (script);
	}	
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
		temp = ft_strjoin(path_list[i++], "/");
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
	}
	return (get_full_path_util(path_list, path, temp, cmd));
}
