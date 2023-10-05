/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utiles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:46:02 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/05 12:00:58 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	creat_exp_list(t_static *s)
{
	int		i;
	t_list	*new;
	t_list	*temp;

	i = 0;
	new = NULL;
	temp = NULL;
	if(s->env)
		s->exp = ft_lstnew(ft_strjoin("declare -x ", s->env->content));
	if (s->env->next)
		temp = s->env->next;
	while (temp)
	{
		char *aux = ft_strjoin("declare -x ", temp->content);
		
		//printf("aux: %p\n", aux);
		//atexit(leaks);	
		new = ft_lstnew(aux);
		add_list_and_sort(&(s->exp), new);

		//leaks();
		
		temp = temp->next;
	}
}

void	print_exp(void)
{
	t_list		*temp;
	t_static	*s;
	char		*value;

	s = ft_get_static();
	temp = s->exp;
	while (temp)
	{
		value = ft_strchr(temp->content, '=');
		if (value && ft_strlen(value) == 1)
			printf("%s\n", ft_substr(temp->content, 0, ft_strlen(temp->content)
					- 1));
		else
			printf("%s\n", temp->content);
		temp = temp->next;
	}
}

int	error_parsing_export(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier'\n", STDERR_FILENO);
	errno = 1;
	return (0);
}

int	error_ft_parsing(char *str)
{
	error_parsing_export(str);
	if (*str == '-')
		errno = 3;
	return (0);
}

/* Each argument for the export cmd 
can only contain alphanumeric character or '_' or '='
and the numeric characters 
and '=' can't be at the beginning of the argument*/
int	ft_parsing(char *str)
{
	int	i;
	int	vocal;

	i = 0;
	vocal = 0;
	if (*str == '=' || *str == '-')
		return (error_ft_parsing(str));
	while (str[i] && str[i] != '=')
	{
		if (str[i] == ' ')
			return (error_parsing_export(str));
		else if (!ft_isalpha(str[i]))
		{
			if (ft_isdigit(str[i]) && !vocal)
				return (error_parsing_export(str));
			else if (str[i] != '_')
				return (error_parsing_export(str));
		}
		else if (ft_isalpha(str[i]))
			vocal += 1;
		i++;
	}
	return (1);
}
