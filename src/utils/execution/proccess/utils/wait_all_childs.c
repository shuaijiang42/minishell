/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_all_childs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:54:05 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 18:54:20 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_wait_all_children(int pid)
{
	int	id;
	int	status;
	int	final;

	status = 0;
	waitpid(pid, &status, 0);
	final = status;
	while (1)
	{
		id = waitpid(-1, &status, 0);
		if (id < 0)
			break ;
	}
	exit(WEXITSTATUS(final));
	pid = 0;
}
