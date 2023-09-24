/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_pipex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:56:09 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:37 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_get_cmd_pipex(char **cmd)
{
	size_t	nxt_cmd;
	char	*str;

	nxt_cmd = get_next_index_pipex(*cmd);
	if (nxt_cmd)
		nxt_cmd -= 1;
	str = ft_strndup(*cmd, nxt_cmd);
	*cmd += get_next_index_pipex(*cmd);
	return (str);
}
