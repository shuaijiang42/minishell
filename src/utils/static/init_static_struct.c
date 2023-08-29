/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_static_struct.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:34:53 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/29 16:15:06 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_static *init_static_struct(char **env)
{
    t_static *s;
    char pwd[4096];
    
    s = malloc(sizeof(t_static));
    if (!s)
        return (NULL);
    
    getcwd(pwd, sizeof(pwd));
    s->oldpwd = NULL;
    s->pwd = ft_strjoin("PWD=ft_strdup(pwd); 
    s->last_cmd = "./minishell";
    s->shlvl = 1 
    if(!env)
    {
        s->env = get_default_env();
        s->exp = get_default_exp();
}
    }
    else
        s->env = get_env_cpy(env);
    