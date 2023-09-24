/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:25:47 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:31:35 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	get_redir(t_argument *content)
{
	char	*str;
	int		i;

	i = 0;
	if (content->str)
		str = cut_input(content->str, &i);
	else
		return ;
	if (i)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
		ft_free((void **)&content->str);
		content->str = NULL;
		return ;
	}
	ft_free((void **)&content->str);
	content->str = str;
	return ;
}
