/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 16:53:23 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/04 18:03:51 by shujiang         ###   ########.fr       */
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
	if (ft_strcmp(input[i], "-n") == 0)
	{
		i++;
		if (!input[i])
			return ;		
	}
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

/* int	ft_built_in(char **input)
{
	if (ft_strcmp(input[0], "echo" == 0))
		ft_echo (input);
	else if (ft_strcmp(input[0], "cd" == 0))
		ft_cd (input);
	else if (ft_strcmp(input[0], "pwd" == 0))
		ft_pwd (input);
	else if (ft_strcmp(input[0], "export" == 0))
		ft_export (input);
	else if (ft_strcmp(input[0], "unset" == 0))
		ft_unset (input);
	else if (ft_strcmp(input[0], "env" == 0))
		ft_env (input);
	else if (ft_strcmp(input[0], "exit" == 0))
		ft_exit (input);
	else
		return (0);
	return (1);
} */
