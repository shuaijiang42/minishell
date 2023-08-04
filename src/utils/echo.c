/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 16:53:23 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/04 17:24:42 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void ft_echo(char    **input)
{
	int i;

	i = 1;
	if (input[i] == NULL)
	{
		printf("\n");
		return ;
	}
	if (ft_strcmp(input[i], "-n") == 0  
			&& input[i + 1] == NULL)
		return ;
	i++;
	while (input[i] && input[i + 1])
	{
		printf("%s ", input[i]);
		i++;
	}
	if (ft_strcmp(input[1], "-n") == 0)
		printf("%s", input[i]);
	else
		printf("%s\n", input[i]);
}
