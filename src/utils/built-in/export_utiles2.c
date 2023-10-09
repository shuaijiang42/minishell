/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utiles2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:47:40 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/09 15:12:08 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	upper_str(char *str1, char *str2)
{
	size_t	i;
	size_t	j;

	if (!str1 && str2)
		return (ft_strlen(str2));
	else if (!str2 && str1)
		return (ft_strlen(str1));
	else if (!str1 && !str2)
		return (0);
	i = ft_strlen(str1);
	j = ft_strlen(str2);
	if (i > j)
		return (i);
	else
		return (j);
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

static void	fill_print_list(char **printed, int len)
{
	int	i;

	i = 0;
	while (i < len)
		printed[i++] = 0;
}

int	in_the_list(char *new, char **list, int len)
{
	size_t	i;

	i = 0;
	while (list[i] && i < (size_t)len)
	{
		if (!ft_strncmp(new, list[i++], ft_strlen(new)))
			return (1);
	}
	return (0);
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
	while (tmp)
	{
		if (tmp->content)
		{
			if (ft_strncmp(max, tmp->content, \
			upper_str(max, tmp->content)) < 0)
				max = tmp->content;
		}
		tmp = tmp->next;
	}
	tmp = env;
	while (tmp)
	{
		if (tmp->content)
		{
			if (ft_strncmp(max, tmp->content, \
			upper_str(max, tmp->content)) < 0)
				max = tmp->content;
		}
		tmp = tmp->next;
	}
	return (max);
}

void	print_min(t_list *env, char **list, int len)
{
	char	*min;
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = env;
	if (!env || !env->content)
		return ;
	min = get_max(env);
	if (!min)
		return ;
	while (tmp)
	{
		if (ft_strncmp(min, tmp->content, \
		upper_str(min, tmp->content)) > 0 && \
		!in_the_list(tmp->content, list, len))
			min = tmp->content;
		tmp = tmp->next;
	}
	while (list[i] && i < len)
		i++;
	list[i] = min;
	while (env)
	{
		if (env->content == min)
			break ;
		env = env->next;
	}
	if (env)
		print_exp_print_content(env);
}

void	print_exp(void)
{
	t_list	*tmp;
	char	**printed;
	int		list_len;

	tmp = (ft_get_static())->env;
	list_len = ft_lstsize(tmp);
	if (!list_len)
		return ;
	printed = malloc(sizeof(char *) * (list_len + 1));
	fill_print_list(printed, list_len);
	printed[list_len] = NULL;
	while (tmp)
	{
		print_min((ft_get_static())->env, printed, list_len);
		tmp = tmp->next;
	}
	ft_free((void **)&printed);
}
