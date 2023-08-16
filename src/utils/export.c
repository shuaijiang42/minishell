/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 18:49:20 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/16 15:35:00 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	creat_exp_list(char **env, t_static *s)
{	
	int i;
	t_list *new;
    t_list *temp;
    
    i = 1;
    new = NULL;
    temp = NULL;
    if (env == NULL)
    {
       printf("Error: No env\n");
       exit (1);
    }
    s->exp = ft_lstnew(ft_strjoin("declare -x ", env[0]));
    temp = s->exp;
    while(env[i])
    {
        new = ft_lstnew(ft_strjoin("declare -x ", env[i]));
        ft_lstadd_back(&temp, new);
        i++;
    }
}

void    print_exp(void)
{
    int i;
    t_list *temp;
    t_static *s;

	s = ft_get_static();
    i = 0;
    temp = s->exp;
    while(temp)
    {
		if (temp->content)
        	printf("%s\n", temp->content);
        temp = temp->next;
    }
}
/* Each argument for the export cmd can only contain alphanumeric character or '_' or '='
	and the numeric characters and '=' can't be at the beginning of the argument*/
int	ft_parsing(char	*str)
{
	int i;

	i = 0;
	while (str[i])
	{	
		if ((!ft_isalnum((int)str[i])) 
			&& ((i == 0 && str[i] != '_') || (i != 0 && str[i] != '=')))
		{
			printf("minishell: export: %c: not a valid identifier\n", str[i]);
			//free;
			return (0);
		}
		i++;
	}
	return (1);
}

int	var_existed(char *str)
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
	while(str[i] && str[i] != '=')
		i++;
	len = i;
	var = ft_substr(str, 0, len);
    while(temp)
    {
		exp = temp->content;
        if(exp && ft_strncmp(exp + 11, var, len) == 0)
		{
			
			return (len);	
		}
        temp = temp->next;
    }
	return (0);
}

/* void add_new_var_exp(char *str)
{
	t_list *new;
	t_list *temp;
	t_static *s;

	s = ft_get_static();
	temp = s->exp;
	new = ft_lstnew(ft_strjoin("declare -x ", str));
    ft_lstadd_back(&temp, new);
} */

void add_new_var_exp(char *str)
{
	t_list *new;
	t_list *temp;
	char *new1;
	char *new2;
	char *new3;
	t_static *s;

	s = ft_get_static();
	new1 = NULL;
	new2 = NULL;
	new3 = NULL;
	if (ft_strlen(ft_strchr(str, '=')) == 1)
	{
		new1 = ft_strjoin("declare -x ", str);
		new2 = ft_strjoin(new1, "\"\"");
		free (new1);
	}
	else
	{
		new1= ft_substr(str, 0, var_existed(str) + 1);
		new2 = ft_strjoin("declare -x ", new1);
		free (new1);
		new1 = ft_strjoin(new2, "\"");
		free (new2);
		new2 = ft_substr(str, var_existed(str) + 2, ft_strlen(str) - var_existed(str) + 1);
		new3 = ft_strjoin(new2, "\"");
		free (new2);
		new2 = ft_strjoin(new1 , new3);
	}	
	temp = s->exp;
	new = ft_lstnew(new2);
    ft_lstadd_back(&temp, new);
}

void add_new_var_env(char *str)
{
	t_list *new;
	t_list *temp;
	char *new1;
	char *new2;
	t_static *s;

	s = ft_get_static();
	new1 = NULL;
	new2 = NULL;
	temp = s->env_cpy;
	new = ft_lstnew(str);
    ft_lstadd_back(&temp, new);
}

void	modify_exp(char *str)
{
	t_list *temp;
	char *old;
	t_static *s;
	char *new1;
	char *new2;
	char *new3;
	
	s = ft_get_static();
	temp = s->exp;
	new1 = NULL;
	new2 = NULL;
	new3 = NULL;
	while(temp)
	{
		old = temp->content;
		if (ft_strncmp(old + 11, str, 5) == 0)
			break ;
		temp = temp->next;
	}
	if (temp)
	{
		new1= ft_substr(old, 0, 11 + var_existed(str) + 1);
		new2 = ft_strjoin(new1, "\"");
		free (new1);
		new1 = ft_strjoin(str + var_existed(str) + 1, "\"");
		temp->content = ft_strjoin(new2, new1);
		free (new1);
		free (new2);
	}
}
		
void	modify_env(char *str)
{
	t_list *temp;
	char *old;
	t_static *s;
	
	s = ft_get_static();
	temp = s->env_cpy;
	while(temp)
	{
		old = temp->content;
		if(ft_strncmp(old, str, 5) == 0)
			break ;
		temp = temp->next;
	}
	if (temp)
		temp->content = str;
}
void	ft_export(char **input)
{
	int i;
	char *var;
	
	i = 1;
	var = NULL;
	if (!input[i])
	{
		print_exp();	
		return ;
	}
	while (input[i])
	{
		if (ft_parsing(input[i]) == 1)
		{
			var = ft_lexer(input[i])[0];
			if (!var_existed(var))
			{
				if (ft_strchr(var, '='))
					add_new_var_env(var);
				add_new_var_exp(var);
			}
			else
			{
				modify_exp(var);
				modify_env(var);
			}
		}
		i++;
	}
}
