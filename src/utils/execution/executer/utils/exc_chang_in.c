/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_chang_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:26:31 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:26:47 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_change_input(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("inp");
	get_redir(content);
	if (!content->str)
		return (-1);
	fd = open(content->str, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
		return (-1);
	}
	lex->in = fd;
	return (fd);
}
