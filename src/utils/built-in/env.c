/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:04:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/08 19:17:07 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	print_env_cpy(void)
{
	t_list	*env;
	t_list	*tmp;

	env = (ft_get_static())->env;
	tmp = env;
	while (tmp)
	{
		if (tmp->content)
		{
			if (have_any_equal(tmp->content))
				printf("%s\n", tmp->content);
		}
		tmp = tmp->next;
	}
}

void	ft_env(char **input)
{
	if (input[1] != NULL)
	{
		printf("env: %s: No such file or directory\n", input[1]);
		return ;
	}
	print_env_cpy();
}
