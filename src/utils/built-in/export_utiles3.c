/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utiles3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 13:09:09 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/10 13:13:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	loop_get_max(t_list *tmp, char **max)
{
	while (tmp)
	{
		if (tmp->content)
		{
			if (ft_strncmp(*max, tmp->content, \
			upper_str(*max, tmp->content)) < 0)
				*max = tmp->content;
		}
		tmp = tmp->next;
	}
}

char	*get_max(t_list	*env)
{
	t_list	*tmp;
	char	*max;

	tmp = env;
	while (tmp && !tmp->content)
		tmp = tmp->next;
	if (!tmp)
		return (NULL);
	max = tmp->content;
	loop_get_max(tmp, &max);
	tmp = env;
	loop_get_max(tmp, &max);
	return (max);
}

static void	print_exp_print_content(t_list *tmp)
{
	char	*str;

	str = get_var_from_node(tmp);
	if (str)
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		write(STDOUT_FILENO, tmp->content, \
		ft_strlen(tmp->content) - ft_strlen(str));
		if (have_any_equal(tmp->content))
		{
			write(STDOUT_FILENO, "\"", 1);
			write(STDOUT_FILENO, str, ft_strlen(str));
			write(STDOUT_FILENO, "\"", 1);
		}
		write(STDOUT_FILENO, "\n", 1);
	}
	ft_free((void **)&str);
}

void	print_min_printer(t_list *env, char *min)
{
	while (env)
	{
		if (env->content == min)
			break ;
		env = env->next;
	}
	if (env)
		print_exp_print_content(env);
}
