/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:09:21 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/09 12:50:48 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_cmd(char *argv);
char	*get_full_path_util(char **path_list, char *path, \
char *temp, char *cmd);
char	*get_full_path(char **env, char *cmd);

char	*cmd_path(char *argv, char **env)
{
	char	*cmd;

	cmd = get_cmd(argv);
	if (ft_strncmp("/", cmd, 1) == 0 || ft_strncmp("./", cmd, 2) == 0
		|| ft_strncmp("../", cmd, 3) == 0)
		return (cmd);
	return (get_full_path(env, cmd));
}
