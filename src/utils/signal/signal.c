/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:56:33 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/11 21:01:41 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	handler(int signal)
{
	if (flag != HERE)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		if (flag == SHELL)
			rl_redisplay();
	}
	else
		exit(2);
	return ;
	signal = 0;
}

void	quit_signal(int signal)
{
	if (flag == SHELL)
		rl_redisplay();
	return ;
	signal = 0;
}
