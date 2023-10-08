/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 18:05:01 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/08 16:29:12 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	unset_var(char *var)
{
	t_list	*tmp1;
	t_list	*tmp2;
	t_list	*node;
	t_list	*env;

	env = ft_get_static()->env;
	tmp2 = env;
	node = search_node_env(var, env);
	if (!node)
		return ;
	tmp1 = node->next;
	while (tmp2)
	{
		if (tmp2->next == node)
			break ;
		tmp2 = tmp2->next;
	}
	if (!tmp2)
		return ;
	ft_free((void **)&node->content);
	ft_free((void **)&node);
	tmp2->next = tmp1;
}

void	ft_unset(char **input)
{
	int	i;

	i = 1;
	if (input[i] == NULL)
		return ;
	while (input[i])
	{
		if (!input[i][0])
		{
			ft_putstr_fd("minishell: unset: `': not a valid identifier\n",
				STDERR_FILENO);
			errno = 1;
			return ;
		}
		if (!is_valid_option(input[i]))
		{
			not_valid_iden_error(input[i]);
			return ;
		}
		unset_var(input[i]);
		i++;
	}
}
