/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 18:49:20 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/09 12:20:49 by samusanc         ###   ########.fr       */
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

void	ft_export_var(char *input)
{
	t_list	*tmp;
	t_list	*tmp2;
	char	*str;
	char	*var;

	tmp = ft_lstnew(input);
	if (!tmp)
		return ;
	str = get_var_from_node(tmp);
	if (!str)
	{
		ft_free((void **)&tmp);
		return ;
	}
	var = ft_strndup(input, ft_strlen(input) - ft_strlen(str));
	if (!var)
	{
		ft_free((void **)&str);
		ft_free((void **)&tmp);
		return ;
	}
	tmp2 = search_node_env(var, (ft_get_static())->env);
	if (!tmp2)
		ft_lstadd_back(&(ft_get_static()->env), ft_lstnew(ft_strdup(input)));
	else
	{
		if (have_any_equal(input))
			change_content_env(tmp2, ft_strdup(input));
	}
	ft_free((void **)&tmp);
	ft_free((void **)&str);
	ft_free((void **)&var);
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
