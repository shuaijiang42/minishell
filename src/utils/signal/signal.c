/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:56:33 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/22 13:11:23 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/* void    handler_sigaction(int signal)
{
    if (signal == SIGINT)
    
   
} */

/* void    ft_sigaction(void)
{
    struct sigaction action;
    
    action.sa_handler = handler_sigaction;
    action.sa_flags = SA_RESTART;
    sigemptyset(&action.sa_mask);
    sigaction(SIGQUIT, &action, NULL);   
    sigaction(SIGINT, &action, NULL);
} */

void    handler(int signal)
{
    char *line;
    
    line = NULL;
    if (signal == SIGINT)
    {
        rl_on_new_line();
        rl_redisplay();
    }
    else if (signal == SIGQUIT)
    {
        return ;
    }
}



void    ft_sigaction(void)
{
    struct sigaction action;
  
    action.sa_handler = handler;
    action.sa_flags = SA_RESTART;
    sigemptyset(&action.sa_mask);
    sigaction(SIGQUIT, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    
}


/* int main()
{
    ft_sigaction();
    while (1)
    {
        printf("hola\n");
        sleep(1);
       
    }
    return (0);
} */


