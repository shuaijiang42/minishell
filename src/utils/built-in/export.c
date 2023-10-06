/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 18:49:20 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/05 13:47:21 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_modify(char *str, t_list *node)
{
	char	*new1;
	char	*new2;
	char	*old;

	old = node->content;
	new1 = ft_substr(old, 0, 11 + var_len(str) + 1);
	if (!ft_strchr(old, '='))
		new2 = ft_strjoin(new1, "=\"");
	else
		new2 = ft_strjoin(new1, "\"");
	free(new1);
	new1 = ft_strjoin(str + var_len(str) + 1, "\"");
	node->content = ft_strjoin(new2, new1);
	free(new1);
	free(new2);
}

void	modify_exp(char *str)
{
	t_list		*temp;
	char		*old;
	t_static	*s;

	s = ft_get_static();
	temp = s->exp;
	while (temp)
	{
		old = temp->content;
		if (ft_strncmp(old + 11, str, var_len(str)) == 0 && ((old
					+ 11)[var_len(str)] == '\0' || (old
				+ 11)[var_len(str)] == '='))
			break ;
		temp = temp->next;
	}
	if (temp)
		ft_modify(str, temp);
}

void	modify_env(char *str)
{
	t_list		*temp;
	char		*old;
	t_static	*s;

	s = ft_get_static();
	temp = s->env;
	while (temp)
	{
		old = temp->content;
		if (ft_strncmp(old, str, var_len(str) + 1) == 0)
			break ;
		temp = temp->next;
	}
	if (temp)
		temp->content = str;
}

void	ft_export_var(char *input)
{
	char	*var;
	char	*old;

	var = ft_strdup(input);
	old = var_existed(var);
	if (!old)
	{
		if (var && ft_strchr(var, '='))
			add_new_var_env(var);
		add_new_var_exp(var);
	}
	else
	{
		if (ft_strchr(var, '='))
		{
			modify_exp(var);
			if (!ft_strchr(old, '='))
				add_new_var_env(var);
			else
				modify_env(var);
		}
		//leaks();
	}
	free(var);
}

void	ft_export(char **input)
{
	int			i;
	t_static	*s;

	s = ft_get_static();
	i = 1;
	if (!input[i])
	{
		print_exp();
		return ;
	}
	while (input[i])
	{
		if (ft_parsing(input[i]) == 1)
			ft_export_var(input[i]);
		else
		{
			if (errno != 3)
				errno = 1;
			else
				errno = 2;
			return ;
		}
		i++;
	}
}
