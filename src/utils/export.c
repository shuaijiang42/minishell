/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 18:49:20 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/10 20:33:42 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	creat_exp_list(char **env)
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
    g->exp = ft_lstnew(ft_strjoin("declare -x ", env[0]));
    temp = g->exp;
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
    
    i = 0;
    temp = g->exp;
    while(temp)
    {
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
			&& ((i = 0 && str[i] != '_') || (i != 0 && str[i] != '=')))
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
    
    i = 0;
    temp = g->exp;
	while(str[i] && str[i] != '=')
		i++;
	len = i - 1;
	var = ft_substr(str, 0, len);
    while(temp)
    {
		exp = temp->content;
        if(ft_strncmp(exp + 11, var, len) == 0)
			return (1);	
        temp = temp->next;
    }
	return (0);
}
void add_new_var_exp(char *str)
{
	t_list *new;
	t_list *temp;
	
	temp = ft_lstlast(g->exp);
	new = ft_lstnew(ft_strjoin("declare -x ", str));
    ft_lstadd_back(&temp, new);
	printf("222222222\n");
}

void add_new_var_env(char *str)
{
	t_list *new;
	t_list *temp;
	char *new1;
	char *new2;

	new2 = NULL;
	temp = ft_lstlast(g->env_cpy);
	if (ft_strlen(ft_strchr(str, '=')) == 1)
		ft_strjoin(str, "\"\"");
	else
	{
		new1 = ft_strjoin("\"", str);
		new2 = ft_strjoin(new1, "\"");
		free(new1);
	}	
	temp = ft_lstlast(g->exp);
	new = ft_lstnew(new2);
    ft_lstadd_back(&temp, new);
	printf("333333333\n");
}

void	modify_exp(char *str)
{
	t_list *temp;
	char *old;
	
	temp = g->exp->content;
	while(temp)
	{
		old = temp->content;
		if(ft_strcmp(old + 11, str) == 0)
			break ;
		temp = temp->next;
	}
	temp->content = ft_strjoin("declare -x ", str);
	free(old);
}
void	modify_env(char *str)
{
	t_list *temp;
	char *old;
	
	temp = g->env_cpy->content;
	while(temp)
	{
		old = temp->content;
		if(ft_strcmp(old, str) == 0)
			break ;
		temp = temp->next;
	}
	temp->content = str;
	free(old);
}
void	ft_export(char **input)
{
	int i;
	
	i = 1;
	if (!input[i])
	{
		print_exp();	
		return ;
	}
	printf("nahhhhhhh\n");
	while (input[i])
	{
		printf("i= %d, input: %s\n", i, input[i]);
		printf("%d\n", ft_parsing(input[i]));
		if (ft_parsing(input[i]) == 1)
		{
			printf("%d\n", ft_parsing(input[i]));
			if (!var_existed(input[i]))
			{
				printf("hiiiiiiii\n");
				if(ft_strchr(input[i], '='))
				{
					printf("holaaaa\n");
					add_new_var_env(input[i]);
				}
				printf("ciaoooooo\n");
				add_new_var_exp(input[i]);
			}
			else
			{
				printf("11111111\n");
				modify_exp(input[i]);
				modify_env(input[i]);
			}
		}
		printf("i = %d\n", i);
		if (i > 100)
			break ;
		i++;		
	}
}
