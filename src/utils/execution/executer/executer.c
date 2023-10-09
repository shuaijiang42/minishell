/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/09 12:21:10 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	executer(char *cmd, t_input *input)
{
	int			cloud[2];
	int			value;
	char		**env;
	t_static	*s;

	s = ft_get_static();
	env = NULL;
	env = list_to_matrix(s->env);
	cloud[0] = dup(0);
	cloud[1] = dup(1);
	value = ft_executer_exec(input, env);
	dup2_with_error_check(cloud[1], 1);
	close(cloud[1]);
	dup2_with_error_check(cloud[0], 0);
	close(cloud[0]);
	ft_free((void **)&cmd);
	ft_free_split_2(&env);
	return (value);
}
