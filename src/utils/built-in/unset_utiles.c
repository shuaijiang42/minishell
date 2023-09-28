/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utiles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:51:16 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:01:47 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	get_var_index_exp(char *var_name)
{
	int			len;
	t_list		*temp;
	int			index;
	char		*ref;
	t_static	*s;

	s = ft_get_static();
	index = 0;
	temp = s->exp;
	len = ft_strlen(var_name);
	ref = temp->content + 11;
	while (temp)
	{
		if (ft_strncmp(ref, var_name, len - 1) == 0 && ref[len] == '=')
			break ;
		temp = temp->next;
		if (temp)
			ref = temp->content + 11;
		index++;
	}
	if (!temp)
		return (-1);
	return (index);
}

int	get_var_index_env(char *var_name)
{
	int			len;
	t_list		*temp;
	int			index;
	char		*ref;
	t_static	*s;

	s = ft_get_static();
	index = 0;
	temp = s->env;
	len = ft_strlen(var_name);
	ref = temp->content;
	while (temp)
	{
		if (ft_strncmp(ref, var_name, len - 1) == 0 && ref[len] == '=')
			break ;
		temp = temp->next;
		if (temp)
			ref = temp->content;
		index++;
	}
	if (!temp)
		return (-1);
	return (index);
}

int	is_valid_option(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (!ft_isalpha(str[i]))
		{
			if (!ft_isdigit(str[i]))
			{
				if (str[i] != '_')
					return (0);
			}
		}
		i++;
	}
	return (1);
}

void	unset_var(char *var_name, int index, t_list *list)
{
	t_list		*node_to_remove;
	t_static	*s;

	s = ft_get_static();
	node_to_remove = ft_locate_node(list, var_name);
	ft_remove_node(list, index);
}

void	not_valid_iden_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	errno = 1;
}
