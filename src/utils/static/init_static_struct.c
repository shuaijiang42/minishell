/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_static_struct.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:34:53 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/19 16:02:57 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char *ft_get_info_from_env(char **env, char *var_name)
{
    int i;
    int len;

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

t_static *init_static_struct(char **env)
{
    t_static *s;
    int *i;
    char pwd[4096];
    //int shlvl;
    
    s = calloc(1, sizeof(t_static));
    if(!s)
	{
        perror("calloc: ");
		return (NULL);
	}
    i = malloc(sizeof(int));
	if (!i)
	{
        perror("malloc: ");
		return (NULL);
	}
	*i = 0;
	s->error = ft_lstnew((void *)i);
    s->pwd = ft_lstnew((void *)getcwd(pwd, sizeof(pwd)));
	s->here = dup(STDIN_FILENO);
	if (s->here == -1)
	{
		perror("dup: ");
		return (NULL);
	} 
    if (!(*env))
    {
        s->oldpwd = ft_lstnew("OLDPWD");
        s->last_cmd = ft_lstnew("_=./minishell");
        s->shlvl = ft_lstnew("SHLVL=1");
    }
    else
    {
        for (int i = 0; env[i]; i++)
        {
            if (!ft_strncmp(env[i], "SHLVL=", 6))
            {
                int nb = ft_atoi(ft_substr(env[i], 6, ft_strlen(env[i])));
                nb++;
                char *new_nb = ft_itoa(nb);
                env[i] = ft_strjoin("SHLVL=", new_nb);
            }
        }
        s->oldpwd = ft_lstnew(ft_get_info_from_env(env, "OLDPWD="));
        s->last_cmd = ft_lstnew(ft_get_info_from_env(env, "_="));
       /*  shlvl = ft_atoi(ft_get_info_from_env(env, "SHLVL=") + 6) + 1;
        printf("shlvl %d\n", shlvl); 
        s->shlvl = ft_lstnew(ft_strjoin("SHLVL=",ft_itoa(shlvl)));
        printf("s-shlvl: %s\n", s->shlvl->content); */
    }
    return (s);
}
   
   
