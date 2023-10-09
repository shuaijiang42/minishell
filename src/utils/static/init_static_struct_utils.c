/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_static_struct_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 12:27:24 by samusanc          #+#    #+#             */
/*   Updated: 2023/10/09 12:28:52 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_get_info_from_env(char **env, char *var_name)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	if (var_name)
		len = ft_strlen(var_name);
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, len) == 0)
			return (env[i]);
		i++;
	}
	return (NULL);
}

void	change_content_env(t_list *list, char *content)
{
	if (!list)
		return ;
	ft_free((void **)&list->content);
	list->content = (void *)content;
}

//if is the variable return 1, else return 0
size_t	compare_var(char *var, char *node)
{
	char	*str;

	str = NULL;
	if (!var || !node)
		return (0);
	if (!ft_strlen(var) || !ft_strlen(node))
		return (0);
	if (!ft_strncmp(var, node, ft_strlen(var)))
		return (1);
	if (ft_strlen(var))
	{
		if (var[ft_strlen(var) - 1] == '=')
		{
			str = ft_strdup(var);
			str[ft_strlen(var) - 1] = '\0';
			if (!ft_strncmp(var, node, ft_strlen(node)))
				return (1);
		}
	}
	ft_free((void **)&str);
	return (0);
}

//search one node if not found return NULL
t_list	*search_node_env(char *str, t_list *env)
{
	size_t	i;
	t_list	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		if (tmp->content && str)
		{
			if (compare_var(str, tmp->content))
				break ;
		}
		tmp = tmp->next;
	}
	return (tmp);
}

//get var from node and malloc the str
char	*get_var_from_node(t_list *node)
{
	size_t	i;
	char	*content;

	i = 0;
	content = (char *)node->content;
	if (!node || !node->content)
		return (NULL);
	while (content[i] && content[i] != '=')
		i++;
	if (content[i])
		i++;
	return (ft_strdup(content + i));
}
