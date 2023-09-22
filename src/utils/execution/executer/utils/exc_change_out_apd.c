/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_change_out_apd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:28:19 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:28:39 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_change_output_apd(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("apd");
	get_redir(content);
	if (!content->str)
		return (-1);
	fd = open(content->str, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
		return (-1);
	}
	lex->out = fd;
	return (fd);
}
