/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 16:53:23 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/11 19:34:04 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


char *ft_get_var(char *var)
{
	t_static *s;
	char *env_var;
	size_t len;
	t_list *temp; 
	
	s = ft_get_static();
	len = ft_strlen(var);
	temp = s->env;
	while (temp)
	{
		env_var = temp->content;
		if(env_var && var && !ft_strncmp(env_var, var, len) 
			&& env_var[ft_strlen(var)]== '=')
			return (env_var + len + 1);
		temp = temp->next;
	}
	return (NULL);
}

void update_dir_for_exp(t_static *s)
{
	t_list  *oldpwd;
	t_list	*old;
	t_list	*new;
	t_list	*new_oldpwd;
	char buf[4096];

	old = NULL;
	new = ft_lstnew(ft_strjoin("declare -x PWD=", getcwd(buf, sizeof(buf))));
	old = ft_locate_node(s->exp, "declare -x PWD");
	new_oldpwd = ft_lstnew(ft_strjoin("declare -x OLD",old->content + 11));
	oldpwd = ft_locate_node(s->exp, "declare -x OLDPWD");
	ft_node_substitute(&oldpwd, &new_oldpwd);
	ft_node_substitute(&old, &new);
}

void update_dir_for_env(t_static *s)
{
	t_list  *oldpwd;
	t_list	*old;
	t_list	*new;
	t_list	*new_oldpwd;
	char buf[4096];

	old = NULL;
	new = ft_lstnew(ft_strjoin("PWD=", getcwd(buf, sizeof(buf))));
	old = ft_locate_node(s->env, "PWD");
	new_oldpwd = ft_lstnew(ft_strjoin("OLD",old->content));
	oldpwd = ft_locate_node(s->env, "OLDPWD");
	ft_node_substitute(&oldpwd, &new_oldpwd);
	ft_node_substitute(&old, &new);
}

void ft_cd(char *path)
{
	DIR *dir;
	t_static *s;
	
	s = ft_get_static();
	if (!path)
	{
		path = ft_get_var("HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			ft_put_error(1);
			return ;
		}	
		chdir(ft_get_var("HOME"));
		printf("hi\n");
		update_dir_for_env(s);
		update_dir_for_exp(s);
		return ;
	}
	dir = opendir(path);
	if (dir)
	{
		if (access(path, X_OK) == -1)
		{
			printf("minishell: cd: %s: Permission denied\n", path);
			ft_put_error(1);
		}
		else
		{
			chdir(path);
			update_dir_for_env(s);
			update_dir_for_exp(s);
		}	
		closedir(dir);
	}
	else
	{
		printf("minishell: cd: %s: No such file or directory\n", path);
		ft_put_error(1);
	}
}

void ft_pwd(void)
{
	char buf[4096];

	printf("%s\n", getcwd(buf, sizeof(buf)));
}

int	check_only_n(char *str)
{
	int i;

	i = 0;
	if (str[i] != '-')
		return (0);
	i++;
	while (str[i])
	{
		if (str[i] != 'n')
			break ;
		i++;
	}
	if (str[i] == '\0')
	{
		return (1);
	}
	return (0);	
}

void ft_echo(char    **input)
{
	int i;

	i = 1;
	if (input[i] == NULL)
	{
		printf("\n");
		return ;
	}
	while (input[i] && check_only_n(input[i]))
	{
		free(input[i]);
		input[i] = ft_strdup("-n");
		i++;
	}
	i = 1;
	while (ft_strcmp(input[i], "-n") == 0)
	{
		i++;
		if (!input[i])
			return ;		
	}
	while (input[i] && input[i + 1])
	{
		printf("%s ", input[i]);
		i++;
	}
	if (ft_strcmp(input[1], "-n") == 0)
		printf("%s", input[i]);
	else
		printf("%s\n", input[i]);
}

int check_digit(char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void ft_free_exit(char **input, int n)
{
	ft_free_input(input);
	exit (n);
}

void ft_exit(char **input)
{
	int i;

	i = 0; 
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (input && input[1] == NULL)
		ft_free_exit(input, 0);
	if (input[2] != NULL)
		ft_print_error("exit: too many arguments\n", 255);
	else if (input[1])
	{
		if(!check_digit(input[1])) 
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd("exit: ", STDERR_FILENO);
			ft_putstr_fd(input[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument requiered\n", STDERR_FILENO);
			ft_put_error(255);
		}
		else
		{
			i = ft_atoi(input[1]);
			ft_free_exit(input, i);
		}	
	}
	if (!ft_get_proccess())
		printf("yeah closing!!\n");
		//ft_save_history(ft_get_history());
	exit (ft_get_error());
}


int	ft_built_in(char **input)
{
	if (input && ft_strcmp(input[0], "echo") == 0)
		ft_echo (input);
	else if (ft_strcmp(input[0], "cd") == 0)
		ft_cd(input[1]);
	else if (ft_strcmp(input[0], "pwd") == 0)
		ft_pwd ();
	else if (ft_strcmp(input[0], "export") == 0)
		ft_export (input);
	else if (ft_strcmp(input[0], "unset") == 0)
		ft_unset (input);
	else if (ft_strcmp(input[0], "env") == 0)
		ft_env (input);
	else if (ft_strcmp(input[0], "exit") == 0)
		ft_exit (input);  
	else
		return (false);
	return (true);
}

int	ft_excuter(char **input, char **env)
{
	t_bool	built_in;
	int	pid;
	int status;
	
	pid = 0;
	status = 0;
	errno = 0;
	built_in = ft_built_in(input);
	if (built_in == false)
	{
		flag = PROCCESS;
		pid = fork_with_error_check();
		if (pid == 0)
			execve_with_error_check(input, env);
		waitpid(-1, &status, 0);
		ft_free_split_2(&input);
		return (WEXITSTATUS(status));
	}
	else
	{
		ft_free_split_2(&input);
		return (errno);
	}
}
