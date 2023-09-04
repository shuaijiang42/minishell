/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:56:33 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/02 17:13:27 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void    handler(int signal)
{
    if (signal == SIGINT)
    {
        printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
        if (!flag)
			rl_redisplay();
    }
}


