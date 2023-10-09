/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:56:33 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/09 12:11:15 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	handler(int signal)
{
	if (g_flag == PROCCESS)
		errno = 130;
	if (g_flag == 4)
		exit(130);
	if (g_flag != HERE)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		if (g_flag == SHELL)
			rl_redisplay();
	}
	else
		exit(2);
	return ;
	signal = 0;
}

void	quit_signal(int signal)
{
	if (g_flag == SHELL)
		rl_redisplay();
	return ;
	signal = 0;
}
