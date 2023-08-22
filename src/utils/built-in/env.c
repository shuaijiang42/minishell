/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 16:53:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/22 13:27:22 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


void env_copy(char **env, t_static *s)
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
    s->env_cpy = ft_lstnew(env[0]);
    temp = s->env_cpy;
    while(env[i])
    {
        new = ft_lstnew(env[i]);
        ft_lstadd_back(&temp, new);
        i++;
    }
}

void    print_env_cpy(void)
{
    int i;
    t_list *temp;
    t_static *s;

	s = ft_get_static();
    i = 0;
    temp = s->env_cpy;
    while(temp)
    {
        if (temp->content)
            printf("%s\n", temp->content);
        temp = temp->next;
    }
}

void    ft_env(char **input)
{
    if (input[1] != NULL)
	{
		printf("env: %s: No such file or directory\n", input[1]);
        return ;
	}
    print_env_cpy();
}
