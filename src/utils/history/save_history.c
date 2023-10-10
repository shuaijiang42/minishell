/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_history.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 19:49:06 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/10 12:42:15 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*get_home_history(char **env)
{
	char	*str;
	size_t	i;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		if (!ft_strncmp("HOME=", env[i], 5))
			break ;
		i++;
	}
	if (!env[i])
		return (NULL);
	str = *(env + i);
	str += 5;
	return (str);
}

void	ft_get_old_history(char **env, int *fd)
{
	char	*str;
	char	*str2;
	char	*str3;

	str = get_home_history(env);
	if (!str)
		return ;
	str2 = ft_strjoin(str, "/.minishell_history");
	*fd = open(str2, O_CREAT | O_RDWR | O_APPEND, 0644);
	ft_free((void *)&str2);
	if (*fd < 0)
		return ;
	str3 = get_next_line(*fd);
	while (str3)
	{
		str3[ft_strlen(str3) - 1] = '\0';
		if (str3)
			add_history(str3);
		ft_free((void *)&str3);
		str3 = get_next_line(*fd);
	}
}

void	ft_save_history(void)
{
	t_save_history	his;

	his.old_history = NULL;
	his.history = (ft_get_static())->history;
	his.fd = ft_get_history();
	if (his.fd != -1 && his.fd != STDIN_FILENO)
		close(his.fd);
	his.env = list_to_matrix((ft_get_static())->env);
	his.fd = fd_old_history_4_list(his.env, &his);
	ft_free_split_2(&(his.env));
	if (his.fd == -1)
		return ;
	his.len = ft_lstsize(his.old_history);
	his.len = his.len - 501;
	if (his.len < 0)
		his.len = 0;
	len_2_fill(1, his.len);
	ft_put_history(his.fd);
	ft_lstclear(&his.old_history, save_and_clear);
}
