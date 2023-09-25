/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 18:49:20 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/25 16:07:40 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	creat_exp_list(t_static *s)
{	
	int i;
	t_list *new;
    t_list *temp;
    
    i = 0;
    new = NULL;
    temp = NULL;
    
    s->exp = ft_lstnew(ft_strjoin("declare -x ", s->env->content));
	if (s->env->next)
		temp = s->env->next;
    while(temp)
    {
        new = ft_lstnew(ft_strjoin("declare -x ", temp->content));
        add_list_and_sort(&(s->exp), new);
        temp  = temp ->next;
    }
}

void    print_exp(void)
{
    t_list *temp;
    t_static *s;
	char *value;

	s = ft_get_static();
    temp = s->exp;
    while(temp)
    {
		value = ft_strchr(temp->content, '=');
		if (value && ft_strlen(value) == 1)
        	printf("%s\n", ft_substr(temp->content, 0, ft_strlen(temp->content) -  1));
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
	//freee i dkn what but free
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

/* Each argument for the export cmd can only contain alphanumeric character or '_' or '='
	and the numeric characters and '=' can't be at the beginning of the argument*/
int	ft_parsing(char	*str)
{
	int i;
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

int var_len(char *str)
{
	int i;
	
	i = 0;
	if(!str)
		return (0);
	while(str[i] && str[i] != '=')
		i++;
	return (i);
}

char	*var_existed(char *str)
{
	int i;
    t_list *temp;
	int len;
	char *var;
	char *exp;
    t_static *s;

	s = ft_get_static();
    i = 0;
    temp = s->exp;
	if (!str)
		return (NULL);
	len = var_len(str);
	var = ft_substr(str, 0, len);
    while(temp)
    {
		exp = temp->content;
        if(exp && ft_strncmp(exp + 11, var, len) == 0
			&& ((exp + 11)[len] == '\0' || (exp + 11)[len] == '=' ))
			return (exp);
        temp = temp->next;
    }
	return (NULL);
}

char *add_exp_syntax(char *str)
{
	char *new1;
	char *new2;
	char *new3;
	
	new1= ft_substr(str, 0, var_len(str) + 1);
	new2 = ft_strjoin("declare -x ", new1);
	free (new1);
	new1 = ft_strjoin(new2, "\"");
	free (new2);
	new2 = ft_substr(str, var_len(str) + 1, ft_strlen(str) - var_len(str) + 1);
	new3 = ft_strjoin(new2, "\"");
	free (new2);
	new2 = ft_strjoin(new1 , new3);
	return (new2);
}

void add_new_var_exp(char *str)
{
	t_list *new;
	char *new1;
	char *new2;
	t_static *s;

	s = ft_get_static();
	new1 = NULL;
	new2 = NULL;
	if (!str)
		return ;
	if (ft_strchr(str, '=') && ft_strlen(ft_strchr(str, '=')) == 1)
	{
		new1 = ft_strjoin("declare -x ", str);
		new2 = ft_strjoin(new1, "\"\"");
		free (new1);
	}
	else
	{
		if(!ft_strchr(str, '='))
			new2 = ft_strjoin("declare -x ", str);
		else
			new2 = add_exp_syntax(str);
	}
	new = ft_lstnew(new2);
     add_list_and_sort(&(s->exp), new);
}

void add_new_var_env(char *str)
{
	t_list *new;
	t_static *s;

	s = ft_get_static();
	new = ft_lstnew(str);
    add_list_and_sort(&(s->env), new);
}

void ft_modify(char *str, t_list *node)
{
	char *new1;
	char *new2;
	char *old;

	old = node->content;
	new1= ft_substr(old, 0, 11 + var_len(str) + 1);
	if (!ft_strchr(old, '='))
		new2 = ft_strjoin(new1, "=\"");
	else 
		new2 = ft_strjoin(new1, "\"");
	free (new1);
	new1 = ft_strjoin(str + var_len(str) + 1, "\"");
	node->content = ft_strjoin(new2, new1);
	free (new1);
	free (new2);
}

void	modify_exp(char *str)
{
	t_list *temp;
	char *old;
	t_static *s;
	
	s = ft_get_static();
	temp = s->exp;
	while(temp)
	{
		old = temp->content;
		if (ft_strncmp(old + 11, str, var_len(str)) == 0 
			&& ((old + 11)[var_len(str)] == '\0' || (old + 11)[var_len(str)] == '=' ))
			break ;
		temp = temp->next;
	}
	if (temp)
		ft_modify(str, temp);
}

void	modify_env(char *str)
{
	t_list *temp;
	char *old;
	t_static *s;
	
	s = ft_get_static();
	temp = s->env;
	while(temp)
	{
		old = temp->content;
		if(ft_strncmp(old, str, var_len(str)+1) == 0)
			break ;
		temp = temp->next;
	}
	if (temp)
		temp->content = str;
}

void ft_export_var(char *input)
{
	char *var;
	char *old;
	
	var =ft_strdup(input);
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
	}
}

void	ft_export(char **input)
{
	int i;
	
	t_static *s;
	
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
