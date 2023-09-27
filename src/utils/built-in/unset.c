/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 18:05:01 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/26 17:51:50 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_unset_var(char *var)
{
	int index1;
	int index2;
	t_static *s;

	s = ft_get_static();
	index1 = get_var_index_env(var);
	index2 = get_var_index_exp(var);
	if (index1 >= 0)
		unset_var(var, index1, s->env);
	if (index2 >= 0)
		unset_var(var, index2, s->exp);
}

void    ft_unset(char **input)
{
	int i;
	
	i = 1;
	if(input[i] == NULL)
		return ;
	while (input[i])
	{
		if (!input[i][0])
		{
			ft_putstr_fd("minishell: unset: `': not a valid identifier\n", STDERR_FILENO);
			errno = 1;
			return ;
		}
		if (!is_valid_option(input[i]))
		{
			not_valid_iden_error(input[i]);
			return ;
		}
		ft_unset_var(input[i]);
		i++;	
	}	
}
