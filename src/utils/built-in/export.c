/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 18:49:20 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/09 15:07:37 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	have_any_equal(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != '=')
		i++;
	if (str[i])
		return (1);
	return (0);
}

void	init_export_var(t_export_var *ptr)
{
	ptr->tmp = NULL;
	ptr->tmp2 = NULL;
	ptr->str = NULL;
	ptr->var = NULL;
}

int	free_all_export(t_export_var *ptr)
{
	ft_free((void **)&ptr->tmp);
	ft_free((void **)&ptr->str);
	ft_free((void **)&ptr->var);
	return (0);
}

int	ft_export_var(char *input)
{
	t_export_var	ptr;

	init_export_var(&ptr);
	ptr.tmp = ft_lstnew(input);
	if (!ptr.tmp)
		return (free_all_export(&ptr));
	ptr.str = get_var_from_node(ptr.tmp);
	if (!ptr.str)
		return (free_all_export(&ptr));
	ptr.var = ft_strndup(input, ft_strlen(input) - ft_strlen(ptr.str));
	if (!ptr.var)
		return (free_all_export(&ptr));
	ptr.tmp2 = search_node_env(ptr.var, (ft_get_static())->env);
	if (!ptr.tmp2)
		ft_lstadd_back(&(ft_get_static()->env), ft_lstnew(ft_strdup(input)));
	else
	{
		if (have_any_equal(input))
			change_content_env(ptr.tmp2, ft_strdup(input));
	}
	return (0);
}

void	ft_export(char **input)
{
	int			i;
	t_static	*s;

	s = ft_get_static();
	i = 1;
	if (!input[i] || (input[i] && !input[i][0]))
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
