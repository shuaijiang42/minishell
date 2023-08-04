/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 16:53:23 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/03 18:13:15 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
void ft_echo(char    *input)
{
    if(ft_strcmp(input, "echo") == 0)
    {
        printf("\n");
        return ;
    } 
    input = ft_strchr(input, ' ');
    if (ft_strncmp(input + 1, "-n ", 3) == 0)
        printf("%s", input + 4);
    else
        printf("%s\n", input + 1);
}