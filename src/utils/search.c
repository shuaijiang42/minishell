/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 16:53:23 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 16:42:33 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
void ft_pwd(void)
{
	char buf[4096];

	printf("%s\n", getcwd(buf, sizeof(buf)));
}

int check_digit(char *str)
{
	size_t	i;
	int		sign;

	if (!str)
		return (0);
	i = 0;
	sign = 0;
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i])
	{
		while (str[i] && (str[i] == '+' || str[i] == '-'))
		{
			sign += 1;
			i++;
		}
		while (str[i] && (str[i] >= '0' && str[i] <= '9'))
			i++;
		if (str[i] || sign > 1)
			return (0);
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
	{
		if (!ft_get_proccess())
			ft_save_history();
		ft_free_exit(input, ft_get_error());
	}
	if (input[1])
	{
		if(!check_digit(input[1])) 
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd("exit: ", STDERR_FILENO);
			ft_putstr_fd(input[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument requiered\n", STDERR_FILENO);
			ft_put_error(255);
			if (!ft_get_proccess())
				ft_save_history();
			exit (255);
		}
		else if (input[2] != NULL)
		{
			ft_print_error("exit: too many arguments", 1);
			return ;
		}
		else
		{
			i = ft_atoi(input[1]);
			ft_free_exit(input, i);
		}	
	}
	if (!ft_get_proccess())
		ft_save_history();
	exit (ft_get_error());
}
*/
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
	int		pid;
	int		status;

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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utiles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:06:21 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 12:52:56 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_free_exit(char **input, int n)
{
	ft_free_input(input);
	exit (n);
}

void	exit_with_no_argc(char **input)
{
	if (!ft_get_proccess())
		ft_save_history();
	ft_free_exit(input, ft_get_error());
}

void	not_numeric_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": numeric argument requiered\n", STDERR_FILENO);
	ft_put_error(255);
	if (!ft_get_proccess())
		ft_save_history();
	exit (255);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:49:41 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 16:43:36 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_cd(char *path)
{
	DIR			*dir;
	t_static	*s;

	s = ft_get_static();
	if (!path)
	{
		if (!ft_get_var("HOME"))
		{
			ft_err_msg("minishell: cd: HOME not set\n", NULL, NULL);
			return ;
		}
		ft_go_somewhere(s, ft_get_var("HOME"));
		return ;
	}
	dir = opendir(path);
	if (dir)
	{
		if (access(path, X_OK) == -1)
			ft_err_msg("minishell: cd: ", path, ": Permission denied\n");
		else
			ft_go_somewhere(s, path);
		closedir(dir);
	}
	else
		ft_err_msg("minishell: cd: ", path, "No such file or directory\n");
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utiles.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:44:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 16:45:40 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_get_var(char *var)
{
	t_static	*s;
	char		*env_var;
	size_t		len;
	t_list		*temp;

	s = ft_get_static();
	len = ft_strlen(var);
	temp = s->env;
	while (temp)
	{
		env_var = temp->content;
		if (env_var && var && !ft_strncmp(env_var, var, len)
			&& env_var[ft_strlen(var)] == '=')
			return (env_var + len + 1);
		temp = temp->next;
	}
	return (NULL);
}

void	update_dir_for_exp(t_static *s)
{
	t_list	*oldpwd;
	t_list	*old;
	t_list	*new;
	t_list	*new_oldpwd;
	char	buf[4096];

	old = NULL;
	new = ft_lstnew(ft_strjoin("declare -x PWD=", getcwd(buf, sizeof(buf))));
	old = ft_locate_node(s->exp, "declare -x PWD");
	new_oldpwd = ft_lstnew(ft_strjoin("declare -x OLD", old->content + 11));
	oldpwd = ft_locate_node(s->exp, "declare -x OLDPWD");
	ft_node_substitute(&oldpwd, &new_oldpwd);
	ft_node_substitute(&old, &new);
}

void	update_dir_for_env(t_static *s)
{
	t_list	*oldpwd;
	t_list	*old;
	t_list	*new;
	t_list	*new_oldpwd;
	char	buf[4096];

	old = NULL;
	new = ft_lstnew(ft_strjoin("PWD=", getcwd(buf, sizeof(buf))));
	old = ft_locate_node(s->env, "PWD");
	new_oldpwd = ft_lstnew(ft_strjoin("OLD", old->content));
	oldpwd = ft_locate_node(s->env, "OLDPWD");
	ft_node_substitute(&oldpwd, &new_oldpwd);
	ft_node_substitute(&old, &new);  
}

void	ft_go_somewhere(t_static *s, char *somewhere)
{
	chdir(somewhere);
	update_dir_for_env(s);
	update_dir_for_exp(s);
}

void	ft_err_msg(char *s1, char *path, char *s2)
{
	ft_putstr_fd(s1, STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(s2, STDERR_FILENO);
	errno = 1;
	ft_put_error(1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:51:34 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:05:23 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_only_n(char *str)
{
	int	i;

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
		return (1);
	return (0);
}

int	ft_echo_no_argv(char **input)
{
	if (input[1] == NULL)
	{
		printf("\n");
		return (1);
	}
	return (0);
}

void	ft_print_holder(char **input, int i)
{
	if (ft_strcmp(input[1], "-n") == 0)
		printf("%s", input[i]);
	else
		printf("%s\n", input[i]);
}

void	ft_echo(char **input)
{
	int	i;

	i = 1;
	if (ft_echo_no_argv(input))
		return ;
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
	ft_print_holder(input, i);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:04:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 15:30:33 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	get_default_env(t_static *s)
{
	char	buf[4096];

	s->env = ft_lstnew(ft_strjoin("PWD=", getcwd(buf, sizeof(buf))));
	add_list_and_sort(&(s->env), ft_lstnew(s->last_cmd->content));
	add_list_and_sort(&(s->env), ft_lstnew(s->oldpwd->content));
	add_list_and_sort(&(s->env), ft_lstnew("SHLVL=1"));
}

void	ft_copy_env(char **env)
{
	int			i;
	t_list		*new;
	t_static	*s;

	s = ft_get_static();
	i = 1;
	new = NULL;
	if (*env == NULL)
	{
		get_default_env(s);
		return ;
	}
	s->env = ft_lstnew(env[0]);
	while (env[i])
	{
		add_list_and_sort(&(s->env), ft_lstnew(env[i]));
		i++;
	}
}

void	print_env_cpy(void)
{
	t_list		*temp;
	char		*value;
	t_static	*s;

	s = ft_get_static();
	temp = s->env;
	value = NULL;
	while (temp)
	{
		value = ft_strchr(temp->content, '=');
		if (ft_strcmp("OLDPWD", temp->content))
			printf("%s\n", (char *)temp->content);
		temp = temp->next;
	}
}

void	ft_env(char **input)
{
	if (input[1] != NULL)
	{
		printf("env: %s: No such file or directory\n", input[1]);
		return ;
	}
	print_env_cpy();
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:05:15 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:02:37 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_digit(char *str)
{
	size_t	i;
	int		sign;

	if (!str)
		return (0);
	i = 0;
	sign = 0;
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i])
	{
		while (str[i] && (str[i] == '+' || str[i] == '-'))
		{
			sign += 1;
			i++;
		}
		while (str[i] && (str[i] >= '0' && str[i] <= '9'))
			i++;
		if (str[i] || sign > 1)
			return (0);
	}
	return (1);
}

void	ft_exit(char **input)
{
	int	i;

	i = 0;
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (input && input[1] == NULL)
		exit_with_no_argc(input);
	if (input[1])
	{
		if (!check_digit(input[1]))
			not_numeric_error(input[1]);
		else if (input[2] != NULL)
		{
			ft_print_error("exit: too many arguments", 1);
			return ;
		}
		else
		{
			i = ft_atoi(input[1]);
			ft_free_exit(input, i);
		}
	}
	if (!ft_get_proccess())
		ft_save_history();
	exit(ft_get_error());
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 18:49:20 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 14:56:28 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_modify(char *str, t_list *node)
{
	char	*new1;
	char	*new2;
	char	*old;

	old = node->content;
	new1 = ft_substr(old, 0, 11 + var_len(str) + 1);
	if (!ft_strchr(old, '='))
		new2 = ft_strjoin(new1, "=\"");
	else
		new2 = ft_strjoin(new1, "\"");
	free(new1);
	new1 = ft_strjoin(str + var_len(str) + 1, "\"");
	node->content = ft_strjoin(new2, new1);
	free(new1);
	free(new2);
}

void	modify_exp(char *str)
{
	t_list		*temp;
	char		*old;
	t_static	*s;

	s = ft_get_static();
	temp = s->exp;
	while (temp)
	{
		old = temp->content;
		if (ft_strncmp(old + 11, str, var_len(str)) == 0 && ((old
					+ 11)[var_len(str)] == '\0' || (old
				+ 11)[var_len(str)] == '='))
			break ;
		temp = temp->next;
	}
	if (temp)
		ft_modify(str, temp);
}

void	modify_env(char *str)
{
	t_list		*temp;
	char		*old;
	t_static	*s;

	s = ft_get_static();
	temp = s->env;
	while (temp)
	{
		old = temp->content;
		if (ft_strncmp(old, str, var_len(str) + 1) == 0)
			break ;
		temp = temp->next;
	}
	if (temp)
		temp->content = str;
}

void	ft_export_var(char *input)
{
	char	*var;
	char	*old;

	var = ft_strdup(input);
	free(input);
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
	int			i;
	t_static	*s;
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utiles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:46:02 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/04 10:13:17 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

 void	leaks()
{
	system("leaks -q minishell");
} 


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
			printf("%p\n", temp->content);
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utiles2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:47:40 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 16:51:27 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	var_len(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

char	*var_existed(char *str)
{
	t_list		*temp;
	char		*var;
	char		*exp;
	t_static	*s;

	s = ft_get_static();
	temp = s->exp;
	if (!str)
		return (NULL);
	var = ft_substr(str, 0, var_len(str));
	while (temp)
	{
		exp = temp->content;
		if (exp && ft_strncmp(exp + 11, var, var_len(str)) == 0 && ((exp
					+ 11)[var_len(str)] == '\0'
			|| (exp + 11)[var_len(str)] == '='))
		{
			free(var);
			return (exp);
		}		
		temp = temp->next;
	}
	free(var);
	return (NULL);
}

char	*add_exp_syntax(char *str)
{
	char	*new1;
	char	*new2;
	char	*new3;

	new1 = ft_substr(str, 0, var_len(str) + 1);
	new2 = ft_strjoin("declare -x ", new1);
	free(new1);
	new1 = ft_strjoin(new2, "\"");
	free(new2);
	new2 = ft_substr(str, var_len(str) + 1, ft_strlen(str) - var_len(str) + 1);
	new3 = ft_strjoin(new2, "\"");
	free(new2);
	new2 = ft_strjoin(new1, new3);
	free(new1);
	free(new3);
	return (new2);
}

void	add_new_var_exp(char *str)
{
	t_list		*new;
	char		*new1;
	char		*new2;
	t_static	*s;

	s = ft_get_static();
	new1 = NULL;
	new2 = NULL;
	if (!str)
		return ;
	if (ft_strchr(str, '=') && ft_strlen(ft_strchr(str, '=')) == 1)
	{
		new1 = ft_strjoin("declare -x ", str);
		new2 = ft_strjoin(new1, "\"\"");
		free(new1);
	}
	else
	{
		if (!ft_strchr(str, '='))
			new2 = ft_strjoin("declare -x ", str);
		else
			new2 = add_exp_syntax(str);
	}
	new = ft_lstnew(new2);
	add_list_and_sort(&(s->exp), new);
}

void	add_new_var_env(char *str)
{
	t_list		*new;
	t_static	*s;

	s = ft_get_static();
	new = ft_lstnew(str);
	add_list_and_sort(&(s->env), new);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:30:13 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 13:52:11 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_put_new_node_first(t_list **list, t_list *new)
{
	int		len;
	char	*str;
	char	*strnew;

	str = (*list)->content;
	strnew = new->content;
	len = ft_var_len(strnew);
	if (ft_strncmp(str, strnew, len) == 0 && str[len] == '='
		&& strnew[len] == '=')
	{
		ft_node_substitute(list, &new);
		return (1);
	}
	else if (ft_strcmp((*list)->content, new->content) > 0)
	{
		ft_lstadd_front(list, new);
		return (1);
	}
	return (0);
}

// len = ft_var_len(insert) + 1; check until '='
// add or substitute and sort

int	ft_substitute_or_insert(t_list **list, t_list *temp, t_list *new)
{
	int		len;
	char	*str;
	char	*strnew;

	str = (*list)->content;
	strnew = new->content;
	len = ft_var_len(strnew);
	if (ft_strncmp(str, strnew, len) == 0 && str[len] == '='
		&& strnew[len] == '=')
	{
		ft_node_substitute(&(temp->next), &new);
		return (1);
	}
	if (temp->next && ft_strcmp(temp->next->content, new->content) > 0)
	{
		ft_front_insert(&temp, &new);
		return (1);
	}
	return (0);
}

void	add_list_and_sort(t_list **list, t_list *new)
{
	t_list	*temp;

	temp = *list;
	if (check_put_new_node_first(list, new) == 1)
		return ;
	while (temp)
	{
		if (ft_substitute_or_insert(list, temp, new))
			break ;
		temp = temp->next;
		if (!temp)
			ft_lstadd_back(list, new);
	}
}

int	ft_count_node(t_list *list)
{
	int		i;
	t_list	*temp;

	i = 0;
	temp = list;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

char	**list_to_matrix(t_list *list)
{
	char	**matrix;
	int		n;
	int		i;
	t_list	*temp;

	i = 0;
	n = ft_count_node(list);
	temp = list;
	matrix = (char **)malloc(sizeof(char *) * (n + 1));
	if (!matrix)
		return (NULL);
	while (i < n)
	{
		matrix[i] = ft_strdup((char *)temp->content);
		temp = temp->next;
		i++;
	}
	matrix[i] = NULL;
	i = 0;
	return (matrix);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utiles.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:49:09 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/04 10:13:46 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_var_len(char *var)
{
	int	i;

	i = 0;
	if (!var)
		return (0);
	while (var[i])
	{
		if (var[i] == '=')
			break ;
		i++;
	}
	return (i);
}

t_list	*ft_locate_node(t_list *list, char *var_name)
{
	t_list	*temp;
	int		len;
	char	*str;

	if (!list)
	{
		printf("this is failing\n");
		return (NULL);
	}
	len = (int)ft_strlen(var_name);
	temp = list;
	str = NULL;
	while (temp)
	{
		str = temp->content;
		if (!ft_strncmp(str, var_name, len) && (str[len] == '='
				|| str[len] == '\0'))
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

void	ft_remove_node(t_list *list, int index)
{
	t_list	*before;
	int		i;
	t_list	*after;
	t_list	*temp;

	i = 0;
	temp = list;
	while (i < index - 1)
	{
		temp = temp->next;
		i++;
	}
	before = temp;
	temp = temp->next;
	after = temp->next;
	before->next = after;
	free(temp);
}

void custom_free(void *data) {
    free(data);
}

void	ft_node_substitute(t_list **old, t_list **new)
{
//	leaks();
	if (*old && *new)
	{
		printf("%s\n", (*old)->content);
		char *aux = ft_strdup((*new)->content);
		// free((*old)->content);
		(*old)->content = aux;
		//if (ft_strnstr(((*old)->content), "PWD=", ft_strlen((*old)->content)))
	
	}
	ft_lstclear(new, custom_free);
//	leaks();
}

void	ft_front_insert(t_list **front_node, t_list **new)
{
	t_list	*back_node;

	back_node = (*front_node)->next;
	(*front_node)->next = *new;
	(*new)->next = back_node;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:03:03 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/26 17:52:14 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_pwd(void)
{
	char	buf[4096];

	printf("%s\n", getcwd(buf, sizeof(buf)));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 18:05:01 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 13:01:31 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_unset_var(char *var)
{
	int			index1;
	int			index2;
	t_static	*s;

	s = ft_get_static();
	index1 = get_var_index_env(var);
	index2 = get_var_index_exp(var);
	if (index1 >= 0)
		unset_var(var, index1, s->env);
	if (index2 >= 0)
		unset_var(var, index2, s->exp);
}

void	ft_unset(char **input)
{
	int	i;

	i = 1;
	if (input[i] == NULL)
		return ;
	while (input[i])
	{
		if (!input[i][0])
		{
			ft_putstr_fd("minishell: unset: `': not a valid identifier\n",
				STDERR_FILENO);
			errno = 1;
			return ;
		}
		if (!is_valid_option(input[i]))
		{
			not_valid_iden_error(input[i]);
			return ;
		}
		ft_unset_var(input[i]);
		i++;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utiles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:51:16 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:01:47 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	get_var_index_exp(char *var_name)
{
	int			len;
	t_list		*temp;
	int			index;
	char		*ref;
	t_static	*s;

	s = ft_get_static();
	index = 0;
	temp = s->exp;
	len = ft_strlen(var_name);
	ref = temp->content + 11;
	while (temp)
	{
		if (ft_strncmp(ref, var_name, len - 1) == 0 && ref[len] == '=')
			break ;
		temp = temp->next;
		if (temp)
			ref = temp->content + 11;
		index++;
	}
	if (!temp)
		return (-1);
	return (index);
}

int	get_var_index_env(char *var_name)
{
	int			len;
	t_list		*temp;
	int			index;
	char		*ref;
	t_static	*s;

	s = ft_get_static();
	index = 0;
	temp = s->env;
	len = ft_strlen(var_name);
	ref = temp->content;
	while (temp)
	{
		if (ft_strncmp(ref, var_name, len - 1) == 0 && ref[len] == '=')
			break ;
		temp = temp->next;
		if (temp)
			ref = temp->content;
		index++;
	}
	if (!temp)
		return (-1);
	return (index);
}

int	is_valid_option(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (!ft_isalpha(str[i]))
		{
			if (!ft_isdigit(str[i]))
			{
				if (str[i] != '_')
					return (0);
			}
		}
		i++;
	}
	return (1);
}

void	unset_var(char *var_name, int index, t_list *list)
{
	t_list		*node_to_remove;
	t_static	*s;

	s = ft_get_static();
	node_to_remove = ft_locate_node(list, var_name);
	ft_remove_node(list, index);
}

void	not_valid_iden_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	errno = 1;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/29 16:40:26 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	executer(char *cmd, t_input *input)
{
	int			cloud[2];
	int			value;
	char		**env;
	t_static	*s;

	s = ft_get_static();
	env = NULL;
	env = list_to_matrix(s->env);
	cloud[0] = dup(0);
	cloud[1] = dup(1);

	value = ft_executer_exec(input, env);
	dup2_with_error_check(cloud[1], 1);
	close(cloud[1]);
	dup2_with_error_check(cloud[0], 0);
	close(cloud[0]);
	ft_free((void **)&cmd);
	ft_free_split_2(&env);
	return (value);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:21:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:21:48 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*cut_input(char *str, int *i)
{
	char	**split;
	char	*result;

	result = NULL;
	split = ft_lexer(str);
	if (!split)
		return (NULL);
	if (*split)
		result = ft_strdup(*split);
	ft_free_split_2(&split);
	if (!result)
	{
		if (i)
			*i = -1;
		return (str);
	}
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_chang_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:26:31 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:26:47 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_change_input(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("inp");
	get_redir(content);
	if (!content->str)
		return (-1);
	fd = open(content->str, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
		return (-1);
	}
	lex->in = fd;
	return (fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_change_out_apd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:28:19 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:28:39 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_change_output_apd(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("apd");
	get_redir(content);
	if (!content->str)
		return (-1);
	fd = open(content->str, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
		return (-1);
	}
	lex->out = fd;
	return (fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_change_out_trc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:27:24 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:27:44 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_change_output_trc(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("trc");
	get_redir(content);
	if (!content->str)
		return (-1);
	fd = open(content->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
		return (-1);
	}
	lex->out = fd;
	return (fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_clear_content.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:57:53 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:13:47 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_exc_clear_content(t_list **result)
{
	ft_lstclear(result, &ft_exc_free_content);
	return (NULL);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_free_content.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:31:02 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 15:49:30 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_exc_free_content(void *cnt_ptr)
{
	t_argument	*content;

	content = (t_argument *)cnt_ptr;
	if (ft_strncmp(content->type, "cmd", 3))
		close(content->fd);
	ft_free((void **)&content->str);
	ft_free((void **)&content->type);
	free(cnt_ptr);
	return ;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_lex_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:59:31 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 17:59:46 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_lex_word(t_list **result, t_exc_lex *lex)
{
	t_list	*tmp;
	char	*wrd;

	wrd = ft_exc_make_word(lex->input + lex->i);
	tmp = NULL;
	tmp = ft_exc_new_node(wrd, lex->status, lex);
	lex->status = non;
	ft_free((void **)&wrd);
	ft_lstadd_back(result, tmp);
	if (!tmp)
		return (-1);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_make_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:02:28 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:14:41 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_exc_make_cmd(t_list **input)
{
	char		*result;
	char		*tmp1;
	t_list		*ptr;
	t_argument	*tmp2;

	result = NULL;
	tmp1 = NULL;
	ptr = *input;
	while (ptr)
	{
		tmp2 = (t_argument *)ptr->content;
		if (!ft_strncmp(tmp2->type, "cmd", 3))
		{
			tmp1 = result;
			result = ft_good_strjoin(result, "   ");
			ft_free((void **)&tmp1);
			tmp1 = result;
			result = ft_good_strjoin(result, tmp2->str);
			ft_free((void **)&tmp1);
		}
		ptr = ptr->next;
	}
	ft_exc_clear_content(input);
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_make_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:02:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:15:31 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_make_redir(char *cmd, t_input *line)
{
	int		std[2];
	t_list	*input;

	std[0] = STDIN_FILENO;
	std[1] = STDOUT_FILENO;
	input = ft_exc_lex_input(cmd, std);
	if (!input)
	{
		if (errno != 9)
			return (errno);
		else
			return (1);
	}
	line->in = dup(std[0]);
	if (std[0] != STDIN_FILENO)
		line->here = 1;
	else
		line->here = 0;
	line->out = dup(std[1]);
	if (line->in == -1 || line->out == -1)
		return (errno);
	line->cmd = ft_exc_make_cmd(&input);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_make_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:59:04 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:16:10 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_exc_make_word(char *input)
{
	int		i;
	int		j;
	char	*str;
	char	*result;
	t_cmd	cmd;

	i = 0;
	j = 0;
	ft_init_cmd(&cmd);
	while (input[i])
	{
		j = ft_check_char(&cmd, input[i]);
		if (!j || j == -1)
			break ;
		i += 1;
	}
	str = ft_strdup(input);
	str[i] = '\0';
	result = ft_strdup(str);
	ft_free((void **)&str);
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_new_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:58:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:29:36 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	*ft_error_to_new_node(void *content)
{
	ft_exc_free_content(content);
	return (NULL);
}

t_list	*ft_exc_new_node(char *argument, t_redir type, t_exc_lex *lex)
{
	t_list		*result;
	t_argument	*content;

	content = malloc(sizeof(t_argument));
	if (!content)
		return (NULL);
	if (argument)
		content->str = ft_strdup(argument);
	else
		content->str = ft_strdup("");
	content->type = NULL;
	if (type == non)
		content->type = ft_strdup("cmd");
	else
		content->fd = ft_exc_open_fd(content, type, lex);
	if (!content->str || !content->type || content->fd == -1)
		return (ft_error_to_new_node((void *)content));
	result = ft_lstnew((void *)content);
	if (!result)
		return (ft_error_to_new_node((void *)content));
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_open_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:30:22 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:30:37 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_open_fd(t_argument *content, t_redir type, t_exc_lex *lex)
{
	if (type == inp)
		return (ft_exc_change_input(content, lex));
	else if (type == hre)
		return (ft_exc_here_doc(content, lex));
	else if (type == trc)
		return (ft_exc_change_output_trc(content, lex));
	else if (type == apd)
		return (ft_exc_change_output_apd(content, lex));
	return (-1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:03:30 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/29 16:41:25 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_executer_exec(t_input *input, char **env)
{
	int	result;


	result = 0;
	dup2_with_error_check(input->in, STDIN_FILENO);
	close(input->in);
	dup2_with_error_check(input->out, STDOUT_FILENO);
	close(input->out);
	result = ft_exc_execution(input->cmd, env);
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:17:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:27:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_check_parse_normal_char(t_exc_lex *lex, int *space)
{
	if (lex->major > 2 || lex->minor > 2 || lex->redirs > 2)
	{
		err_unexpected(lex->minor, lex->major, lex->first);
		return (-1);
	}
	lex->major = 0;
	lex->minor = 0;
	lex->redirs = 0;
	*space = 0;
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_dup_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:18:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:21:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	loop_check_dup(t_exc_lex *lex, char *input, int *space)
{
	lex->j = ft_check_char(&lex->cmd, input[lex->i]);
	if (!lex->j || lex->j == -1)
	{
		if (!lex->j)
		{
			if (lex->major || lex->minor || lex->redirs)
				*space = 1;
		}
		ft_init_cmd(&lex->cmd);
	}
	else
	{
		if (ft_check_parse_normal_char(lex, space) == -1)
			return (-1);
	}
	if (lex->j == -1 && (input[lex->i] == '<' || input[lex->i] == '>'))
	{
		if (ft_check_parse_redirs(lex, space) == -1)
			return (-1);
	}
	return (0);
}

int	ft_check_dup_redir(char *input)
{
	t_exc_lex	lex;
	int			space;

	space = 0;
	lex.input = input;
	ft_init_exc_lex(&lex);
	while (input[lex.i])
	{
		if (loop_check_dup(&lex, input, &space))
			return (-1);
		lex.i += 1;
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:17:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:28:22 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_check_parse_redirs(t_exc_lex *lex, int *space)
{
	if (*space)
		return (err_unexpected(lex->minor, lex->major, 'A'));
	if (lex->input[lex->i] == '<' && !lex->major)
	{
		if (!lex->redirs)
			lex->first = '<';
		lex->minor += 1;
		lex->redirs += 1;
	}
	else if (lex->input[lex->i] == '<' && lex->major)
		return (err_unexpected(lex->minor, lex->major, lex->first));
	if (lex->input[lex->i] == '>' && !lex->minor)
	{
		if (!lex->redirs)
			lex->first = '>';
		lex->major += 1;
		lex->redirs += 1;
	}
	else if (lex->input[lex->i] == '>' && lex->minor)
		return (err_unexpected(lex->minor, lex->major, lex->first));
	if (lex->major > 2 || lex->minor > 2 || lex->redirs > 2)
		return (err_unexpected(lex->minor, lex->major, lex->first));
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_exc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:15:16 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:28:58 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_error_make_list(t_list **result, t_exc_lex *lex, int error)
{
	ft_init_exc_lex(lex);
	ft_exc_clear_content(result);
	if (error)
		err_unexpected(0, 0, 0);
	return (NULL);
}

int	err_unexpected(int minor, int major, char first)
{	
	if (first == 'A')
	{
		if (minor)
			ft_print_error("syntax error near unexpected token `<'", 258);
		if (major)
			ft_print_error("syntax error near unexpected token `>'", 258);
	}
	else if ((minor > 2) || (major == 2 && !minor))
		ft_print_error("syntax error near unexpected token `<'", 258);
	else if ((major > 2) || (minor == 2 && !major))
		ft_print_error("syntax error near unexpected token `>'", 258);
	else if (minor && !major && first == '<')
		ft_print_error("syntax error near unexpected token `>'", 258);
	else if (major && !minor && first == '>')
		ft_print_error("syntax error near unexpected token `<'", 258);
	else
		ft_print_error("syntax error near unexpected token `newline'", 258);
	return (-1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exc_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:14:28 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:30:03 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_execution(char *cmd, char **env)
{
	char	**input;

	if (!cmd)
		return (0);
	input = ft_lexer(cmd);
	ft_free((void **)&cmd);
	if (!*input)
		return ((int)ft_free_split_2(&input) + 258);
	if (!input[0][0])
	{
		ft_print_error(": command not found", 127);
		return (127);
	}
	else
		return (ft_excuter(input, env));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_exc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:15:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:16:12 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_init_exc_lex(t_exc_lex *lex)
{
	lex->i = 0;
	lex->major = 0;
	lex->minor = 0;
	lex->redirs = 0;
	lex->word = 0;
	lex->space = 1;
	lex->first = '\0';
	lex->status = non;
	lex->in = STDIN_FILENO;
	lex->out = STDOUT_FILENO;
	ft_init_cmd(&lex->cmd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_status_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:19:46 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:30:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_put_status_redir(t_exc_lex *lex, char *input)
{
	input = lex->input;
	ft_init_cmd(&lex->cmd);
	lex->word = INACTIVE;
	if (lex->j == -1)
	{
		if (input[lex->i] == '<')
		{
			if (lex->status == non)
				lex->status = inp;
			else
				lex->status = hre;
		}
		else
		{
			if (lex->status == non)
				lex->status = trc;
			else
				lex->status = apd;
		}
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:20:17 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:31:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	ft_strlen2(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:25:47 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:31:35 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	get_redir(t_argument *content)
{
	char	*str;
	int		i;

	i = 0;
	if (content->str)
		str = cut_input(content->str, &i);
	else
		return ;
	if (i)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
		ft_free((void **)&content->str);
		content->str = NULL;
		return ;
	}
	ft_free((void **)&content->str);
	content->str = str;
	return ;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   good_strjoin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:01:56 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:31:49 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_good_strjoin(char *s1, char*s2)
{
	char	*str;
	char	*ret;

	if (!s1 && !s2)
		return (NULL);
	if (!s1 && s2)
		return (ft_strdup(s2));
	if (s1 && !s2)
		return (ft_strdup(s1));
	str = malloc (ft_strlen(s1) + ft_strlen(s2) + 1);
	ret = str;
	if (!str)
		return (NULL);
	while (*s1)
		*str++ = *s1++;
	while (*s2)
	{
		*str = *s2;
		str++;
		s2++;
	}
	*str = '\0';
	return (ret);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:23:40 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:22:10 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ft_here_doc_owo(t_argument *content, int pipes[2])
{
	char	*str;

	flag = HERE;
	content->str = cut_input(content->str, NULL);
	str = readline("> ");
	if (!str)
		exit (0);
	while (1)
	{
		if (!ft_strncmp(content->str, str, ft_strlen2(content->str) + 1))
			exit(1);
		ft_putstr_fd(str, pipes[1]);
		write(pipes[1], "\n", 1);
		free(str);
		str = readline("> ");
		if (!str)
			exit (0);
	}
	free(str);
	exit(0);
}

int	ft_exc_here_doc(t_argument *content, t_exc_lex *lex)
{
	int		pipes[2];
	int		pid;
	int		status;

	if (pipe(pipes))
		exit(errno);
	content->type = ft_strdup("hre");
	pid = fork();
	if (!pid)
		ft_here_doc_owo(content, pipes);
	waitpid(pid, &status, 0);
	flag = PROCCESS;
	if (WEXITSTATUS(status) == 2)
	{
		flag = 3;
		close(pipes[1]);
		close(pipes[0]);
		errno = 1;
		return (-1);
	}
	close(pipes[1]);
	lex->in = pipes[0];
	return (pipes[0]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:01:34 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:32:25 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*ft_exc_lex_input(char *input, int std[2])
{
	t_exc_lex	lex;
	t_list		*result;

	lex.input = input;
	ft_init_exc_lex(&lex);
	if (!input)
	{
		ft_print_error("syntax error near unexpected token `|'", 257);
		return (NULL);
	}
	else if (ft_check_dup_redir(input) == -1)
		return (NULL);
	result = ft_make_list(&lex);
	std[0] = lex.in;
	std[1] = lex.out;
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:00:23 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 16:04:55 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*ft_make_list(t_exc_lex *lex)
{
	t_list		*result;
	char		*input;

	result = NULL;
	input = lex->input;
	while (input[lex->i])
	{
		lex->j = ft_check_char(&lex->cmd, input[lex->i]);
		if (!lex->j || lex->j == -1)
			ft_put_status_redir(lex, NULL);
		else
		{
			if (lex->word == INACTIVE)
			{
				if (ft_exc_lex_word(&result, lex) == -1)
					return (ft_error_make_list(&result, lex, 0));
				lex->word = ACTIVE;
			}
		}
		lex->i += 1;
	}
	if (lex->status)
		return (ft_error_make_list(&result, lex, 1));
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proccess.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:16:47 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/29 16:38:54 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	proccess_pipex(char *cmd)
{
	int	pid;
	int	status;

	flag = PROCCESS;
	pid = fork_with_error_check();
	ft_put_proccess(1);
	if (!pid)
	{
		flag = 4;
		pipex(cmd);
	}
	waitpid(-1, &status, 0);
	ft_put_error(WEXITSTATUS(status));
}

static void	proccess_solo(char *cmd)
{
	int		pid;
	t_input	line;

	pid = ft_exc_make_redir(cmd, &line);
	if (pid)
	{
		ft_put_error(pid);
		return ;
	}
	ft_is_valid_in(STDIN_FILENO, &line);
	ft_put_error(executer(cmd, &line));
}

void	ft_procces_maker(char *cmd, char **env)
{
	char	**input;

	input = ft_lexer(cmd);
	if (input)
	{
		ft_free_split_2(&input);
		if (count_pipes(cmd) > 0)
			proccess_pipex(cmd);
		else
			proccess_solo(cmd);
	}
	else
		ft_free_split_2(&input);
	env = NULL;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:51:45 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:09 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	count_pipes(char *str)
{
	int		j;
	int		i;
	int		n;
	t_cmd	cmd;

	j = 4;
	i = 0;
	n = 0;
	ft_init_cmd(&cmd);
	while (str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == -1)
		{
			if (str[i] == '<' || str[i] == '>')
				ft_init_cmd(&cmd);
			else if (!str[i])
				break ;
			else
				n += 1;
		}
		i++;
	}
	return (n);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:52:17 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:20:36 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_first_child(char *cmd, int pipe[2])
{
	int		pid;
	t_input	input;

	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		return (pipex_error_make_redir(pipe));
	pid = fork_with_error_check();
	if (!pid)
	{
		close(pipe[0]);
		dup2_with_error_check(pipe[1], input.out);
		close(pipe[1]);
		exit(executer(cmd, &input));
	}
	close(pipe[1]);
	return (pipe[0]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:55:32 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:23 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_strndup(const char *s1, size_t n)
{
	char	*str;
	size_t	i;
	size_t	j;

	str = malloc(sizeof(char) * (n + 1));
	if (!str || !s1 || !n)
		return (ft_free((void **)&str));
	i = 0;
	j = 0;
	str[n] = '\0';
	while (i < n && s1[i])
		str[i++] = s1[j++];
	return (str);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_pipex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:56:09 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:37 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_get_cmd_pipex(char **cmd)
{
	size_t	nxt_cmd;
	char	*str;

	nxt_cmd = get_next_index_pipex(*cmd);
	if (nxt_cmd)
		nxt_cmd -= 1;
	str = ft_strndup(*cmd, nxt_cmd);
	*cmd += get_next_index_pipex(*cmd);
	return (str);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_index.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:51:15 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:59 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	get_next_index_pipex(char *str)
{
	int		j;
	int		i;
	int		n;
	t_cmd	cmd;

	j = 4;
	i = 0;
	n = 0;
	ft_init_cmd(&cmd);
	while (str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == -1)
		{
			if (str[i] == '<' || str[i] == '>')
				ft_init_cmd(&cmd);
			else if (!str[i])
				break ;
			else
				return (i + 1);
		}
		i++;
	}
	return (i + 1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:54:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:20:58 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	pipex_error_make_redir(int pipe[2])
{
	if (flag == 3)
		exit(1);
	else
	{
		close (pipe[1]);
		return (pipe[0]);
	}
}

int	ft_last_child(char *cmd, int fd)
{
	int		pipe[2];
	int		pid;
	t_input	input;

	pipe_with_error_check(pipe);
	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		return (pipex_error_make_redir(pipe));
	pid = fork_with_error_check();
	if (!pid)
	{
		close(pipe[1]);
		ft_is_valid_in(fd, &input);
		close(fd);
		exit(executer(cmd, &input));
	}
	close(fd);
	close(pipe[1]);
	ft_wait_all_children(pid);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mid_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:53:29 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:21:23 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_mid_child(char *cmd, int fd)
{
	int		pipe[2];
	int		pid;
	t_input	input;

	pipe_with_error_check(pipe);
	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		return (pipex_error_make_redir(pipe));
	pid = fork_with_error_check();
	if (!pid)
	{
		ft_is_valid_in(fd, &input);
		close(fd);
		dup2_with_error_check(pipe[1], input.out);
		close(pipe[1]);
		exit(executer(cmd, &input));
	}
	close(input.in);
	close(input.out);
	close(fd);
	close(pipe[1]);
	return (pipe[0]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:56:48 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:45:49 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static	void	pipex_init_pipex(t_pipstr *pipex, int *fd, char *cmd)
{
	*fd = 0;
	pipex->i = 1;
	pipex->cmd_cpy = cmd;
	pipex->n = count_pipes(cmd);
	pipex->status = 0;
}

void	pipex(char *cmd)
{
	t_pipstr	pipex;
	int			pid;
	int			fd;
	int			status;

	status = 0;
	pid = fork_with_error_check();
	if (!pid)
	{
		pipex_init_pipex(&pipex, &fd, cmd);
		pipe_with_error_check(pipex.pipes.start_pipe);
		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		fd = ft_first_child(pipex.cmd, pipex.pipes.start_pipe);
		while (pipex.i < pipex.n)
		{
			pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
			fd = ft_mid_child(pipex.cmd, fd);
			pipex.i += 1;
		}
		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		pipex.status = ft_last_child(pipex.cmd, fd);
	}
	waitpid(-1, &status, 0);
	exit (WEXITSTATUS(status));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:52:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:25:19 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_is_valid_in(int fd, t_input *input)
{
	char	**split;
	size_t	i;
	int		pipes[2];

	i = 0;
	split = ft_lexer(input->cmd);
	if (!split)
		return ;
	while (split[i])
		i++;
	ft_free_split_2(&split);
	if (i == 1 && input->here)
	{
		pipe_with_error_check(pipes);
		close(pipes[1]);
		dup2_with_error_check(input->in, pipes[0]);
		return ;
	}
	dup2_with_error_check(fd, input->in);
	return ;
	(void)pipes;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_all_childs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:54:05 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 18:54:20 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_wait_all_children(int pid)
{
	int	id;
	int	status;
	int	final;

	status = 0;
	waitpid(pid, &status, 0);
	final = status;
	while (1)
	{
		id = waitpid(-1, &status, 0);
		if (id < 0)
			break ;
	}
	exit(WEXITSTATUS(final));
	pid = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 16:21:28 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 20:13:01 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_get_old_history(char **env, int *fd)
{
	int		i;
	char	*str;
	char	*str2;
	char	*str3;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		if (!ft_strncmp("HOME=", env[i], 5))
			break ;
		i++;
	}
	if (!env[i])
		return ;
	str = *(env + i);
	str += 5;
	str2 = ft_strjoin(str, "/.minishell_history");
	*fd = open(str2, O_CREAT | O_RDWR | O_APPEND, 0644);
	ft_free((void *)&str2);
	if (*fd < 0)
		return ;
	str3 = get_next_line(*fd);
	while(str3)
	{
		str3[ft_strlen(str3) - 1] = '\0';
		if (str3)
			add_history(str3);
		ft_free((void *)&str3);
		str3 = get_next_line(*fd);
	}
}

int	len_2_fill(int x, int new)
{
	static int	value = 0;

	if (x)
		value = new;
	return (value);
}

void	save_and_clear(void *content)
{
	char		*str;
	static int	i = 0;

	str = (char *)content;
	if (!str)
		return ;
	if (*str && (!((ft_get_history()) == STDIN_FILENO)))
	{
		if (i >= len_2_fill(0, 0))
		{
			ft_putstr_fd(str, ft_get_history());
			ft_putstr_fd("\n", ft_get_history());
		}
	}
	i++;
	ft_free((void **)&str);
	return ;
}

char	*ft_get_history_path(char **env)
{
	size_t	i;
	char	*str;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		if (!ft_strncmp("HOME=", env[i], 5))
			break ;
		i++;
	}
	if (!env[i])
		return (NULL);
	str = *(env + i);
	str += 5;
	return (ft_strjoin(str, "/.minishell_history"));
}

void	ft_save_history_in_list(int fd, t_list **list)
{
	char	*str;

	str = get_next_line(fd);
	while (str)
	{
		str[ft_strlen(str) - 1] = '\0';
		ft_lstadd_back(list, ft_lstnew((void *)ft_strdup(str)));
		ft_free((void *)&str);
		str = get_next_line(fd);
	}
	return ;
}

int	fd_old_history_4_list(char **env, t_save_history *his)
{
	int		fd;
	char	*str2;

	str2 = ft_get_history_path(env);
	if (!str2)
		return (-1);
	fd = open(str2, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
	ft_save_history_in_list(fd, &his->old_history);
	ft_lstadd_back(&his->old_history, his->history);
	close(fd);
	fd = open(str2, O_CREAT | O_RDWR | O_TRUNC, 0644);
	ft_free((void *)&str2);
	return (fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_history.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 19:49:06 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 19:27:23 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_save_history(void)
{
	t_save_history	his;
	
	his.old_history = NULL;
	his.history = (ft_get_static())->history;
	his.fd = ft_get_history();
	if (his.fd != -1 && his.fd != STDIN_FILENO)
		close(his.fd);
	his.env = list_to_matrix((ft_get_static())->env);
	his.fd = fd_old_history_4_list(his.env, &his);
	ft_free_split_2(&(his.env));
	if (his.fd == -1)
		return ;
	his.len = ft_lstsize(his.old_history);
	his.len = his.len - 501;
	if (his.len < 0)
		his.len = 0;
	len_2_fill(1, his.len);
	ft_put_history(his.fd);
	ft_lstclear(&his.old_history, save_and_clear);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   holaaaa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:11:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:26:38 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

char	**ft_lexer(char *str)
{
	char			**result;
	int				len;

	if (ft_check_argument(str) == -1)
		return (NULL);
	len = ft_lexer_len_n_arguments(str);
	result = ft_calloc(sizeof(char *), (len + 1));
	if (!result)
		return (NULL);
	result[len] = NULL;
	ft_alloc_parse_result(&result, str, len);
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:05:02 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:57:48 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	count_arguments(char *str)
{
	int				i;
	int				command;
	int				n_commands;

	if (!str)
		return (0);
	i = 0;
	n_commands = 0;
	command = 0;
	while (str[i] == ' ')
		i++;
	if (str[i])
		n_commands = 1;
	while (str[i])
	{
		while (str[i] != ' ' && str[i])
			i++;
		command = 0;
		while (str[i] == ' ' && str[i])
			i++;
		if (str[i] && str[i] != ' ' && !command)
			command += n_commands++;
		i++;
	}
	return (n_commands);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_delimiter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:06:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:06:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	dollar_delimiter(char c)
{
	if (!c || c == '$' || c == '\'' || c == '\"' \
		|| c == '|' || c == '<' || c == '>')
		return (0);
	return (1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_argument.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:12:10 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:12:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//			ft_check_argument
//		return values:
//		1 if is a valid argument 
//		0 if is a empty argument
//		-1 if is a invalid argument

int	ft_check_argument(char *str)
{
	int	len;

	len = ft_get_next_command(str);
	if (len == -4)
		return (0);
	while (len > 0)
	{
		str += len;
		len = ft_get_next_command(str);
	}
	if (len == -1)
		return (-1);
	return (1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:37:30 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:52:04 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_delimiters(t_cmd *cmd, char c);

int	ft_check_char(t_cmd *cmd, char c)
{
	if (c == '\'' || c == '\"')
		return (ft_lex_quotes(cmd, c));
	else if (c == '$')
		return (ft_lex_dollar(cmd, c));
	else if (c == '|' || c == '<' || c == '>')
		return (ft_lex_delimiters(cmd, c));
	else if (!c)
		return (-1);
	else if (c == ' ')
		return (ft_lex_space(cmd, c));
	else if (c == '?')
		return (ft_lex_interrogation(cmd, c));
	else
		return (ft_lex_chars(cmd, c));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dollar_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:52:37 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:20:27 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ft_dollar_fill_init(t_dollar_fill *strc, int *x, char *dst)
{
	strc->j = 4;
	strc->i = 0;
	strc->z = 0;
	strc->x = x;
	strc->dst = dst;
	strc->tmp = (ft_get_static())->env;
}

static int	ft_dollar_fill_util(t_dollar_fill *strc)
{
	if (!strc->tmp)
	{
		if (strc->str2[0] == '?')
		{
			strc->str3 = ft_itoa(ft_get_error());
			ft_strlcpy(strc->dst, strc->str3, ft_strlen(strc->str3) + 1);
			strc->i = ft_strlen(strc->str3);
			strc->x[0] += strc->i;
			ft_free((void **)&strc->str3);
			ft_free((void **)&strc->str2);
			return (0);
		}
		else
			strc->dst[0] = '\0';
		ft_free((void **)&strc->str2);
		return (0);
	}
	return (1);
}

static int	ft_dollar_fill_util_2(t_dollar_fill *strc, char *str)
{
	if (strc->i == 1)
	{
		strc->dst[0] = '$';
		strc->x[0] += 1;
		return (0);
	}
	strc->str2 = malloc(sizeof(char) * (strc->i + 1));
	if (!strc->str2)
		return (0);
	ft_strlcpy(strc->str2, str, strc->i);
	strc->str2[strc->i] = '\0';
	return (1);
}

static int	ft_dollar_checkin(t_dollar_fill *strc, char *str, t_cmd	*cmd)
{
	if (!str)
		return (0);
	if (!str[0])
	{
		strc->dst[0] = '$';
		strc->x[0] += 1;
		return (0);
	}
	strc->j = ft_check_char(cmd, str[strc->i++]);
	return (1);
}

void	ft_dollar_fill(char *str, t_cmd cmd, int *x, char *dst)
{
	t_dollar_fill	strc;

	ft_dollar_fill_init(&strc, x, dst);
	if (!ft_dollar_checkin(&strc, str, &cmd))
		return ;
	while (strc.j == 3)
		strc.j = ft_check_char(&cmd, str[strc.i++]);
	if (!ft_dollar_fill_util_2(&strc, str))
		return ;
	while (strc.tmp)
	{
		if (!ft_strncmp((char *)strc.tmp->content, \
		strc.str2, ft_strlen(strc.str2)))
			break ;
		strc.tmp = strc.tmp->next;
	}
	if (!ft_dollar_fill_util(&strc))
		return ;
	strc.str3 = (char *)strc.tmp->content + strc.i;
	ft_strlcpy(dst, strc.str3, ft_strlen(strc.str3) + 1);
	*x += ft_strlen(strc.str3);
	return ;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dollar_len.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:50:05 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 16:43:26 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	dollar_return(t_dollar_len *dollar, int n)
{
	dollar->return_value = n;
	return (-1);
}

static void	init_init_dollar_len(char *str, t_cmd cmd, \
t_dollar_len *dollar)
{
	dollar->j = 4;
	dollar->i = 0;
	dollar->cmd = cmd;
	dollar->str = str;
	dollar->j = ft_check_char(&dollar->cmd, dollar->str[dollar->i]);
	dollar->i += 1;
	dollar->return_value = 0;
	dollar->tmp = (ft_get_static())->env;
	dollar->str2 = NULL;
}

static int	init_dollar_len(char *str, t_cmd cmd, t_dollar_len *dollar)
{
	if (!str)
		return (dollar_return(dollar, 0));
	if (!str[0])
		return (dollar_return(dollar, 1));
	init_init_dollar_len(str, cmd, dollar);
	while (dollar->j == 3)
	{
		dollar->j = ft_check_char(&dollar->cmd, dollar->str[dollar->i]);
		dollar->i += 1;
	}
	if (dollar->i == 1)
		return (dollar_return(dollar, 1));
	dollar->str2 = malloc(sizeof(char) * (dollar->i + 1));
	if (!dollar->str2)
		return (dollar_return(dollar, 0));
	ft_strlcpy(dollar->str2, dollar->str, dollar->i);
	dollar->str2[dollar->i] = '\0';
	return (0);
}

static int	dollar_flags(t_dollar_len *dollar)
{
	if (dollar->str2[0] == '?')
	{
		ft_free((void **)&dollar->str2);
		dollar->str2 = ft_itoa(ft_get_error());
		dollar->i = ft_strlen(dollar->str2);
		ft_free((void **)&dollar->str2);
		return (dollar->i);
	}
	ft_free((void **)&dollar->str2);
	return (1);
}

int	ft_dollar_len(char *str, t_cmd cmd)
{
	t_dollar_len	dollar;

	if (init_dollar_len(str, cmd, &dollar) == -1)
		return (dollar.return_value);
	while (dollar.tmp)
	{
		if (!ft_strncmp((char *)dollar.tmp->content, \
		dollar.str2, ft_strlen(dollar.str2)))
			break ;
		dollar.tmp = dollar.tmp->next;
	}
	if (!dollar.tmp)
		return (dollar_flags(&dollar));
	ft_free((void **)&dollar.str2);
	return (ft_strlen(((char *)dollar.tmp->content + dollar.i)) + 1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:14:38 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:21:02 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_free_split_2(char ***split)
{
	int	i;

	i = 0;
	while (split[0][i])
	{
		if (split[0][i])
			ft_free((void *)&split[0][i]);
		i++;
	}
	ft_free((void *)&split[0]);
	*split = NULL;
	return (NULL);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:10:48 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 18:06:07 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	errors_get_next_cmd(int x, t_get_next_command *cmd, int n);
static int	init_get_next_command(char *str, t_get_next_command *cmd);
static int	get_next_return_util(t_get_next_command *cmd, int x);
static int	loop_get_next_cmd(t_get_next_command *cmd);
int			ft_get_next_command(char *str);

/*
			ft_get_next_command
		return values:
		-4 if the command is empty
		-3 if is the last or only command
		-2 if is one pipe open
		-1 if the quotes is no closed
		(other value) index of the next command in the array
*/
int	ft_get_next_command(char *str)
{
	t_get_next_command	cmd;

	if (init_get_next_command(str, &cmd) == -1)
		return (cmd.return_status);
	while (cmd.str[cmd.i] && !cmd.end)
	{
		while (cmd.status.status == q_open && cmd.str[cmd.i])
		{
			if (loop_get_next_cmd(&cmd))
				break ;
		}
		cmd.n_commands += 1;
		while (cmd.str[cmd.i] == ' ' && cmd.str[cmd.i] && !cmd.end)
			cmd.i += 1;
		if ((str[cmd.i] == '|' || str[cmd.i] == '<' || str[cmd.i] == '>'))
			return (get_next_return_util(&cmd, 1));
		if (cmd.str[cmd.i] && !cmd.end)
		{
			if ((str[cmd.i] == '|' || str[cmd.i] == '<' || str[cmd.i] == '>'))
				break ;
			cmd.status.status = q_open;
		}
	}
	return (get_next_return_util(&cmd, 0));
}

static int	errors_get_next_cmd(int x, t_get_next_command *cmd, int n)
{
	if (x == 1)
		ft_print_error("syntax error near unexpected token `|'", 258);
	else if (x == 2)
		ft_print_error("syntax error unclosed quotes", 69);
	cmd->return_status = n;
	return (-1);
}

static int	init_get_next_command(char *str, t_get_next_command *cmd)
{
	if (!str)
		return (errors_get_next_cmd(0, cmd, -1));
	cmd->str = str;
	cmd->status.dollar = funtional;
	cmd->status.simple_q = q_close;
	cmd->status.double_q = q_close;
	cmd->status.dollar = funtional;
	cmd->i = 0;
	cmd->n_commands = 0;
	cmd->command = 0;
	cmd->end = 0;
	while (cmd->str[cmd->i] == ' ')
		cmd->i += 1;
	if (cmd->str[cmd->i] == '|')
		return (errors_get_next_cmd(1, cmd, -1));
	if (str[cmd->i])
		cmd->status.status = q_open;
	else
		return (errors_get_next_cmd(0, cmd, -4));
	return (0);
}

static int	get_next_return_util(t_get_next_command *cmd, int x)
{
	if (x)
	{
		if (!cmd->str[cmd->i + 1])
			return (-2);
		else
			return (cmd->i + 1);
	}
	else
	{
		if (!cmd->str[cmd->i] && cmd->status.simple_q == q_close && \
		cmd->status.double_q == q_close)
			cmd->status.status = q_close;
		if (cmd->status.status == q_open)
			return (errors_get_next_cmd(2, cmd, -1));
		return (-3);
	}
}

static int	loop_get_next_cmd(t_get_next_command *cmd)
{
	if (ft_lexer_check_status(&cmd->status, cmd->str, &cmd->i))
	{
		cmd->end = 1;
		return (1);
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:48:30 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:49:08 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_init_cmd(t_cmd *cmd)
{
	cmd->quotes = no_q;
	cmd->dollar_status = q_close;
	cmd->status = q_close;
	cmd->spaces = 1;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_chars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:34:23 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:20:36 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_chars(t_cmd *cmd, char c)
{
	if (cmd->status == q_close)
		cmd->status = q_open;
	if (cmd->dollar_status == q_open)
	{
		cmd->spaces = 1;
		return (3);
	}
	return (2);
	c = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_delimiters.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:17:59 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:18:23 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_delimiters(t_cmd *cmd, char c)
{
	cmd->dollar_status = q_close;
	if (cmd->status == q_close)
		return (-1);
	else
	{
		if (cmd->quotes == no_q)
		{
			cmd->status = q_close;
			return (-1);
		}
		else
			return (2);
	}
	c = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:35:21 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:21:33 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_dollar(t_cmd *cmd, char c)
{
	if (cmd->quotes != s_q)
	{
		if (cmd->spaces)
		{
			cmd->dollar_status = q_open;
			cmd->spaces = 0;
			return (4);
		}
		else
		{
			cmd->dollar_status = q_close;
			cmd->spaces = 3;
			return (3);
		}
	}
	else
	{
		cmd->dollar_status = q_close;
		cmd->spaces = 0;
		return (2);
	}
	c = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_interrogation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:36:11 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:36:29 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_interrogation(t_cmd *cmd, char c)
{
	if (cmd->dollar_status == q_open)
	{
		cmd->dollar_status = q_close;
		if (cmd->spaces)
		{
			cmd->spaces = 3;
			return (2);
		}
		else
		{
			cmd->spaces = 3;
			return (3);
		}
	}
	else
		return (2);
	c = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:16:27 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:37:33 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	lex_quotes_status_close(t_cmd *cmd, char c);
static int	lex_quotes_status_open_noq(t_cmd *cmd, char c);
static int	lex_quotes_status_open_sq(t_cmd *cmd, char c);
int			ft_lex_quotes(t_cmd *cmd, char c);

//	1 no printable, 
//	2 printable, 
//	0 space and -1 finish or special char
int	ft_lex_quotes(t_cmd *cmd, char c)
{
	if (cmd->status == q_close)
		return (lex_quotes_status_close(cmd, c));
	else
	{
		if (cmd->quotes == no_q)
			return (lex_quotes_status_open_noq(cmd, c));
		else
			return (lex_quotes_status_open_sq(cmd, c));
	}
}

static int	lex_quotes_status_close(t_cmd *cmd, char c)
{
	cmd->status = q_open;
	if (c == '\'')
	{
		cmd->dollar_status = q_close;
		cmd->quotes = s_q;
		cmd->spaces = 3;
	}
	else
		cmd->quotes = d_q;
	return (1);
}

static int	lex_quotes_status_open_noq(t_cmd *cmd, char c)
{
	if (c == '\'')
	{
		cmd->quotes = s_q;
		cmd->dollar_status = q_close;
		cmd->spaces = 3;
	}
	else
	{
		cmd->dollar_status = q_close;
		cmd->quotes = d_q;
		cmd->spaces = 3;
	}
	return (1);
}

static int	lex_quotes_status_open_sq(t_cmd *cmd, char c)
{
	if (cmd->quotes == s_q)
	{
		if (c == '\'')
		{
			cmd->dollar_status = q_close;
			cmd->quotes = no_q;
			cmd->spaces = 3;
			return (1);
		}
		else
			return (2);
	}
	else
	{
		if (c == '\"')
		{
			cmd->quotes = no_q;
			cmd->dollar_status = q_close;
			cmd->spaces = 3;
			return (1);
		}
		else
			return (2);
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_space.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:22:29 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:23:13 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_space(t_cmd *cmd, char c)
{
	if (cmd->status == q_close)
		return (0);
	else
	{
		if (cmd->quotes == no_q)
		{
			cmd->status = q_close;
			cmd->dollar_status = q_close;
			cmd->dollar = funtional;
			return (0);
		}
		else
		{
			if (cmd->quotes == d_q)
			{
				cmd->dollar_status = q_close;
				return (2);
			}
			else
				return (2);
		}
	}
	c = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_check_status.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:08:16 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 18:20:02 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	lex_open_simple_quotes(t_command *cmd)
{
	cmd->simple_q = q_open;
	cmd->status = q_open;
}

static void	lex_open_double_quotes(t_command *cmd)
{
	cmd->double_q = q_open;
	cmd->status = q_open;
}

static void	ch_lexer_are_quotes(t_command *cmd, char *str, int *i)
{
	if (cmd->simple_q == q_open || cmd->double_q == q_open)
	{
		if (str[*i] == '\'' && cmd->simple_q == q_open)
		{
			if (!str[*i + 1] || str[*i + 1] == ' ')
				cmd->status = q_close;
			cmd->simple_q = q_close;
			cmd->dollar = funtional;
		}
		else if (str[*i] == '\"' && cmd->double_q == q_open)
		{
			if (!str[*i + 1] || str[*i + 1] == ' ')
				cmd->status = q_close;
			cmd->double_q = q_close;
		}
	}
	else if (str[*i] == '\'' && cmd->simple_q == q_close \
	&& cmd->simple_q == q_close)
		lex_open_simple_quotes(cmd);
	else if (str[*i] == '\"' && cmd->simple_q == q_close && \
	cmd->simple_q == q_close)
		lex_open_double_quotes(cmd);
}

int	ft_lexer_check_status(t_command *cmd, char *str, int *i)
{
	if (cmd->status == q_open)
	{
		if (cmd->simple_q == q_close && cmd->double_q == q_close \
		&& (str[*i] == '|' || str[*i] == '<' || str[*i] == '>'))
		{
			cmd->status = q_close;
			return (1);
		}
		else if (str[*i] == '\'' || str[*i] == '\"')
			ch_lexer_are_quotes(cmd, str, i);
		else if ((str[*i] == ' ' || !str[*i]) && \
		cmd->simple_q == q_close && cmd->double_q == q_close)
			cmd->status = q_close;
		else if (str[*i] == '$' && cmd->simple_q == q_close)
		{
			if (str[*i + 1] == '$')
				*i += 1;
			while (!str[*i + 1] && dollar_delimiter(str[*i + 1]))
				*i += 1;
		}
		*i += 1;
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_len.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:56:10 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/25 11:21:30 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	init_lexer_len(t_lexer_len *len)
{
	len->len = 0;
	len->i = 0;
	len->j = 0;
	len->x = 0;
	ft_init_cmd(&len->cmd);
}

int	ft_lexer_len_argument(char *str)
{
	t_lexer_len	len;

	init_lexer_len(&len);
	while (!len.j && str[len.i])
	{
		len.j = ft_check_char(&len.cmd, str[len.i]);
		if (len.j == 2)
			len.len += 1;
		if (len.j == 4)
			len.len += ft_dollar_len(str + len.i + 1, len.cmd);
		len.i += 1;
	}
	if (str[len.i] && len.j >= 0)
	{
		while (str[len.i] && len.j > 0)
		{
			len.j = ft_check_char(&len.cmd, str[len.i]);
			if (len.j == 2)
				len.len += 1;
			if (len.j == 4)
				len.len += ft_dollar_len(str + len.i + 1, len.cmd);
			len.i += 1;
		}
	}
	return (len.len);
}

void	ft_lexer_len_loop(t_lexer_len *len, char *str)
{
	len->j = ft_check_char(&len->cmd, str[len->i]);
	if (len->j > 0 && len->x != 1)
	{
		len->len += 1;
		len->x = 1;
	}
	len->i += 1;
}

int	ft_lexer_len_n_arguments(char *str)
{
	t_lexer_len	len;

	init_lexer_len(&len);
	while (str[len.i] && !len.j && len.j != -1)
	{
		len.j = ft_check_char(&len.cmd, str[len.i]);
		if (len.j > 0)
		{
			len.len += 1;
			len.x = 1;
		}
		len.i += 1;
	}
	while (str[len.i] && len.j >= 0)
	{
		while (str[len.i] && len.j > 0 && len.j != -1)
			ft_lexer_len_loop(&len, str);
		len.x = 0;
		ft_init_cmd(&len.cmd);
		while (str[len.i] && !len.j && len.j != -1)
			ft_lexer_len_loop(&len, str);
	}
	return (len.len);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:54:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/25 11:27:01 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_lexer_fill_str(char *str, char **str2)
{
	t_lexer_len	l;

	init_lexer_len(&l);
	while (!l.j && str[l.i])
	{
		l.j = ft_check_char(&l.cmd, str[l.i]);
		if (l.j == 2)
			str2[0][l.x++] = str[l.i];
		if (l.j == 4)
			ft_dollar_fill(str + l.i + 1, l.cmd, &l.x, str2[0] + l.x);
		l.i += 1;
	}
	if (str[l.i] && l.j >= 0)
	{
		while (str[l.i] && l.j > 0)
		{
			l.j = ft_check_char(&l.cmd, str[l.i]);
			if (l.j == 2)
				str2[0][l.x++] = str[l.i];
			if (l.j == 4)
				ft_dollar_fill(str + l.i + 1, l.cmd, &l.x, str2[0] + l.x);
			l.i += 1;
		}
	}
}
// debugg string: printf("[%d] = '%c', j = %d\n", i, str[i], j);

int	ft_lexer_get_next_argument(char *str)
{
	t_cmd			cmd;
	int				len;
	int				i;
	int				j;

	len = 0;
	i = 0;
	j = 0;
	ft_init_cmd(&cmd);
	while (str[i] && !j)
		j = ft_check_char(&cmd, str[i++]);
	while (str[i] && j >= 0)
	{
		while (str[i] && j > 0)
			j = ft_check_char(&cmd, str[i++]);
		return (i);
	}
	return (ft_strlen(str));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:58:27 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/25 11:38:09 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	init_alloc_parse(char ***res, t_alloc_parse *all)
{
	all->i = 0;
	all->x = 0;
	all->arg_len = 0;
	all->result = *res;
}

//	this funtion count how many numbers need per argument
void	ft_alloc_parse_result(char ***result_ptr, char *str, int len)
{
	t_alloc_parse	all;

	init_alloc_parse(result_ptr, &all);
	while (len)
	{
		all.arg_len = ft_lexer_len_argument(str);
		all.str2 = ft_calloc(sizeof(char), all.arg_len + 1);
		if (!all.str2)
			return ;
		ft_lexer_fill_str(str, &all.str2);
		all.str2[all.arg_len] = '\0';
		if (*all.str2)
			all.result[all.x++] = all.str2;
		else
		{
			if (*str != '$')
				all.result[all.x++] = all.str2;
			else
				ft_free((void **)&all.str2);
		}
		all.i = ft_lexer_get_next_argument(str);
		str += all.i;
		len--;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:09:21 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 14:38:37 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**split_path(char **env)
{
	char	*all_path;
	char	**path;
	int		i;

	i = 0;
	all_path = NULL;
	while (env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
			all_path = ft_strdup(env[i] + 5);
		i++;
	}
	if (all_path == NULL)
		return (NULL);
	path = ft_split(all_path, ':');
	free(all_path);
	return (path);
}

void	*ft_free_split(void **strings)
{
	int	i;

	i = 0;
	while (strings && strings[i])
	{
		free(strings[i]);
		i++;
	}
	free(strings);
	return (NULL);
}

char	*get_cmd(char *argv)
{
	char	*cmd;

	cmd = ft_strdup(argv);
	return (cmd);
}
char	*get_full_path_util(char **path_list, char *path, char *temp, char *cmd)
{
	if (path_list)
	{
		ft_free_split_2((char ***)&path_list);
		exit_cmd_not_found(path, temp, cmd);
		return (NULL);
	}
	else
	{
		char *script = ft_strjoin("./", cmd);
		free(cmd);
		return (script);
	}	
}

char	*get_full_path(char **env, char *cmd)
{
	char	**path_list;
	int		i;
	char	*path;
	char	*temp;

	i = 0;
	path = NULL;
	temp = NULL;
	path_list = split_path(env);
	while (path_list && path_list[i])
	{
		temp = ft_strjoin(path_list[i++], "/");
		path = ft_strjoin(temp, cmd);
		if (access(path, X_OK) == 0)
		{
			free(temp);
			if (path_list)
				ft_free_split_2((char ***)&path_list);
			return (path);
		}
		ft_free((void **)&temp);
		ft_free((void **)&path);
	}
	return (get_full_path_util(path_list, path, temp, cmd));
}

char	*cmd_path(char *argv, char **env)
{
	char	*cmd;

	cmd = get_cmd(argv);
	if (ft_strncmp("/", cmd, 1) == 0 || ft_strncmp("./", cmd, 2) == 0
		|| ft_strncmp("../", cmd, 3) == 0)
		return (cmd);
	return (get_full_path(env, cmd));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 12:21:27 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/02 17:50:49 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	pipe_with_error_check(int *fd)
{
	if (pipe(fd) == -1)
	{
		perror("Pipe");
		exit(EXIT_FAILURE);
	}
}

void	dup2_with_error_check(int fd1, int fd2)
{
	if (dup2(fd1, fd2) == -1)
	{
		perror("Dup2");
		exit(EXIT_FAILURE);
	}
}

int	open_with_error_check(const char *path, int oflag)
{
	int	fd;

	fd = open(path, oflag, 0666);
	if (fd == -1)
	{
		perror(path);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

int	fork_with_error_check(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	exit_cmd_not_found(char *path, char *temp, char *cmd)
{
	free(path);
	free(temp);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	errno = 127;
	exit(127);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:10:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/12 17:20:02 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <lexer.h>

void	wait_all_children(void)
{
	int	id;
	int	status;

	status = 0;
	while (1)
	{
		id = waitpid(-1, &status, 0);
		if (id < 0)
			break ;
	}
	exit(status);
}

void	child_execute_preparation(int i, int *end, int argc, char **argv)
{
	int	outfile_fd;

	outfile_fd = 0;
	if (i < argc - 2)
	{
		dup2_with_error_check(end[WRITE], STDOUT_FILENO);
		close(end[WRITE]);
	}
	if (i == argc - 2)
	{
		if (ft_strcmp(argv[1], "here_doc") == 0)
			outfile_fd = open_with_error_check(argv[argc - 1],
					O_WRONLY | O_CREAT | O_APPEND);
		else
			outfile_fd = open_with_error_check(argv[argc - 1],
					O_WRONLY | O_CREAT | O_TRUNC);
		dup2_with_error_check(outfile_fd, STDOUT_FILENO);
	}
}

int	execve_with_error_check(char **argv, char **env)
{
	char	*path;
	char	**arguments;

	arguments = argv;
	if (!arguments)
		exit(0);
	path = cmd_path(*argv, env);
	if (execve(path, argv, env) == -1)
	{
		perror(*argv);
		exit(127);
	}
	exit (0);
	return (0);
}

int	execute_cmd(t_var5 var)
{
	int	end[2];
	int	pid;

	if (var.i < var.argc - 2)
		pipe_with_error_check(end);
	pid = fork_with_error_check();
	if (pid == 0)
	{
		if (var.fd == -1)
			exit(1);
		dup2_with_error_check(var.fd, STDIN_FILENO);
		close(var.fd);
		child_execute_preparation(var.i, end, var.argc, var.argv);
		execve_with_error_check(&var.argv[var.i], var.env);
	}
	if (var.i == var.argc - 2)
	{
		close(end[READ]);
		wait_all_children();
	}
	if (var.fd)
		close(var.fd);
	close(end[WRITE]);
	var.fd = end[READ];
	return (var.fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:07:01 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/03 15:29:28 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	print_error(char *str)
{
	ft_putendl_fd(str, 2);
	exit(1);
}

void	here_doc(char *limiter, int fd)
{
	char	*s;
	char	*limit;

	limit = ft_strjoin(limiter, "\n");
	while (1)
	{
		ft_putstr_fd("here_doc > ", 1);
		s = get_next_line(STDIN_FILENO);
		if (!s)
			exit(1);
		if (ft_strcmp(s, limit) == 0)
		{
			free(s);
			free(limit);
			break ;
		}
		ft_putstr_fd(s, fd);
		free(s);
	}
}

t_var2	get_input_fd_for_cmd1(char **argv)
{
	int		end[2];
	t_var2	var;

	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		var.i = 3;
		pipe_with_error_check(end);
		here_doc(argv[2], end[WRITE]);
		close(end[WRITE]);
		var.fd = end[READ];
	}
	else
	{
		var.i = 2;
		var.fd = open(argv[1], O_RDONLY);
		if (var.fd == -1)
			perror(argv[1]);
	}
	return (var);
}

/*
int	main(int argc, char **argv, char **env)
{
	int		temp;
	t_var5	var5;
	t_var2	var2;

	if (argc < 5)
		print_error("Not enough arguments.");
	var2 = get_input_fd_for_cmd1(argv);
	var5.argc = argc;
	var5.argv = argv;
	var5.env = env;
	var5.i = var2.i;
	var5.fd = var2.fd;
	while (var5.i < var5.argc - 1)
	{	
		temp = execute_cmd(var5);
		var5.i++;
		var5.fd = temp;
	}
	return (0);
}
*/
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:05:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 15:32:25 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_samu.h"

char	*ft_free_samu(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
	return (NULL);
}

char	*ft_error(struct s_data_base *file)
{
	file->saved_line_len = 0;
	ft_free_samu(&file->saved_line);
	return (NULL);
}

int	ft_verify_new_line(struct s_data_base *file)
{
	int	i;

	i = 0;
	if (!file->saved_line)
		return (0);
	while (i != file->saved_line_len)
	{
		if (file->saved_line[i] == '\n' || \
		file->saved_line[i] == '\r')
			return (1);
		++i;
	}
	return (0);
}

char	*ft_read_line(struct s_data_base *file, int *read_number, int fd)
{
	char	*str_for_realloc;

	str_for_realloc = NULL;
	*read_number = read(fd, file->buffer_line, BUFFER_SIZE);
	if (*read_number < 0)
		return (ft_error(file));
	str_for_realloc = ft_realloc(file->saved_line, file->buffer_line, \
	file->saved_line_len, *read_number);
	file->saved_line_len = file->saved_line_len + *read_number;
	ft_free_samu(&file->saved_line);
	file->saved_line = str_for_realloc;
	return (file->saved_line);
}

char	*get_next_line_samu(int fd)
{
	static struct s_data_base	file[FOPEN_MAX];
	int							read_number;
	char						*result;

	read_number = 1;
	if (read(fd, 0, 0) < 0 || fd < 0 \
	|| BUFFER_SIZE < 1 || fd > FOPEN_MAX)
		return (ft_error(&file[fd]));
	while (read_number)
	{
		if (ft_verify_new_line(&file[fd]))
			return (ft_split_lines(&file[fd]));
		if (!ft_read_line(&file[fd], &read_number, fd))
			return (0);
	}
	result = ft_mem_cpy(file[fd].saved_line, file[fd].saved_line_len);
	ft_error(&file[fd]);
	return (result);
}
/*
int	main(int argc, char **argv)
{
	char	*str;
	int		fd;
	int		i = 4;

	fd = open(argv[1], O_RDONLY);
	while (i)
	{
		str = get_next_line(fd);
		printf("The line result:%s", str);
		write(1, "el resultado", 12);
		write(1, str, 100);
		--i;
	}
	//system("leaks -q aoeu");
	return (0);
}
*/
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:16:06 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/31 14:35:58 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line_samu.h"

char	*ft_split_lines(struct s_data_base *file)
{
	int		i;
	char	*string_for_db;
	char	*string_result;

	i = 0;
	while (file->saved_line[i] != '\n' && file->saved_line[i] != '\r')
		++i;
	string_result = ft_mem_cpy(file->saved_line, i + 1);
	string_for_db = ft_mem_cpy((file->saved_line + (i + 1)), \
	(file->saved_line_len - (i + 1)));
	file->saved_line_len = file->saved_line_len - (i + 1);
	ft_free_samu(&file->saved_line);
	file->saved_line = string_for_db;
	return (string_result);
}

char	*ft_mem_cpy(char *str, int len)
{
	char	*result;
	int		i;

	i = 0;
	result = NULL;
	result = malloc(sizeof(char) * ((int)len + 1));
	if (!result || !str || len <= 0)
	{
		if (result)
			ft_free_samu(&result);
		return (NULL);
	}
	while (i <= len)
		result[i++] = '\0';
	i = 0;
	while (i != len)
		result[i++] = *str++;
	return (result);
}

char	*ft_realloc(char *str1, char *str2, int str1_len, int str2_len)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str1 || !str2)
	{
		if (str1)
			return (ft_mem_cpy(str1, str1_len));
		if (str2)
			return (ft_mem_cpy(str2, str2_len));
		return (NULL);
	}
	result = malloc(sizeof(char) * ((int)(str1_len + str2_len + 1)));
	if (!result)
		return (NULL);
	result[(str1_len + str2_len)] = '\0';
	while (i != str1_len)
		result[i++] = *str1++;
	while (j != str2_len)
		result[i++] = str2[j++];
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 16:53:23 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 16:42:33 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
void ft_pwd(void)
{
	char buf[4096];

	printf("%s\n", getcwd(buf, sizeof(buf)));
}

int check_digit(char *str)
{
	size_t	i;
	int		sign;

	if (!str)
		return (0);
	i = 0;
	sign = 0;
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i])
	{
		while (str[i] && (str[i] == '+' || str[i] == '-'))
		{
			sign += 1;
			i++;
		}
		while (str[i] && (str[i] >= '0' && str[i] <= '9'))
			i++;
		if (str[i] || sign > 1)
			return (0);
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
	{
		if (!ft_get_proccess())
			ft_save_history();
		ft_free_exit(input, ft_get_error());
	}
	if (input[1])
	{
		if(!check_digit(input[1])) 
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd("exit: ", STDERR_FILENO);
			ft_putstr_fd(input[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument requiered\n", STDERR_FILENO);
			ft_put_error(255);
			if (!ft_get_proccess())
				ft_save_history();
			exit (255);
		}
		else if (input[2] != NULL)
		{
			ft_print_error("exit: too many arguments", 1);
			return ;
		}
		else
		{
			i = ft_atoi(input[1]);
			ft_free_exit(input, i);
		}	
	}
	if (!ft_get_proccess())
		ft_save_history();
	exit (ft_get_error());
}
*/
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
	int		pid;
	int		status;

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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utiles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:06:21 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 12:52:56 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_free_exit(char **input, int n)
{
	ft_free_input(input);
	exit (n);
}

void	exit_with_no_argc(char **input)
{
	if (!ft_get_proccess())
		ft_save_history();
	ft_free_exit(input, ft_get_error());
}

void	not_numeric_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": numeric argument requiered\n", STDERR_FILENO);
	ft_put_error(255);
	if (!ft_get_proccess())
		ft_save_history();
	exit (255);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:49:41 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 16:43:36 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_cd(char *path)
{
	DIR			*dir;
	t_static	*s;

	s = ft_get_static();
	if (!path)
	{
		if (!ft_get_var("HOME"))
		{
			ft_err_msg("minishell: cd: HOME not set\n", NULL, NULL);
			return ;
		}
		ft_go_somewhere(s, ft_get_var("HOME"));
		return ;
	}
	dir = opendir(path);
	if (dir)
	{
		if (access(path, X_OK) == -1)
			ft_err_msg("minishell: cd: ", path, ": Permission denied\n");
		else
			ft_go_somewhere(s, path);
		closedir(dir);
	}
	else
		ft_err_msg("minishell: cd: ", path, "No such file or directory\n");
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utiles.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:44:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 16:45:40 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_get_var(char *var)
{
	t_static	*s;
	char		*env_var;
	size_t		len;
	t_list		*temp;

	s = ft_get_static();
	len = ft_strlen(var);
	temp = s->env;
	while (temp)
	{
		env_var = temp->content;
		if (env_var && var && !ft_strncmp(env_var, var, len)
			&& env_var[ft_strlen(var)] == '=')
			return (env_var + len + 1);
		temp = temp->next;
	}
	return (NULL);
}

void	update_dir_for_exp(t_static *s)
{
	t_list	*oldpwd;
	t_list	*old;
	t_list	*new;
	t_list	*new_oldpwd;
	char	buf[4096];

	old = NULL;
	new = ft_lstnew(ft_strjoin("declare -x PWD=", getcwd(buf, sizeof(buf))));
	old = ft_locate_node(s->exp, "declare -x PWD");
	new_oldpwd = ft_lstnew(ft_strjoin("declare -x OLD", old->content + 11));
	oldpwd = ft_locate_node(s->exp, "declare -x OLDPWD");
	ft_node_substitute(&oldpwd, &new_oldpwd);
	ft_node_substitute(&old, &new);
}

void	update_dir_for_env(t_static *s)
{
	t_list	*oldpwd;
	t_list	*old;
	t_list	*new;
	t_list	*new_oldpwd;
	char	buf[4096];

	old = NULL;
	new = ft_lstnew(ft_strjoin("PWD=", getcwd(buf, sizeof(buf))));
	old = ft_locate_node(s->env, "PWD");
	new_oldpwd = ft_lstnew(ft_strjoin("OLD", old->content));
	oldpwd = ft_locate_node(s->env, "OLDPWD");
	ft_node_substitute(&oldpwd, &new_oldpwd);
	ft_node_substitute(&old, &new);  
}

void	ft_go_somewhere(t_static *s, char *somewhere)
{
	chdir(somewhere);
	update_dir_for_env(s);
	update_dir_for_exp(s);
}

void	ft_err_msg(char *s1, char *path, char *s2)
{
	ft_putstr_fd(s1, STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(s2, STDERR_FILENO);
	errno = 1;
	ft_put_error(1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:51:34 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:05:23 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_only_n(char *str)
{
	int	i;

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
		return (1);
	return (0);
}

int	ft_echo_no_argv(char **input)
{
	if (input[1] == NULL)
	{
		printf("\n");
		return (1);
	}
	return (0);
}

void	ft_print_holder(char **input, int i)
{
	if (ft_strcmp(input[1], "-n") == 0)
		printf("%s", input[i]);
	else
		printf("%s\n", input[i]);
}

void	ft_echo(char **input)
{
	int	i;

	i = 1;
	if (ft_echo_no_argv(input))
		return ;
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
	ft_print_holder(input, i);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:04:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 15:30:33 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	get_default_env(t_static *s)
{
	char	buf[4096];

	s->env = ft_lstnew(ft_strjoin("PWD=", getcwd(buf, sizeof(buf))));
	add_list_and_sort(&(s->env), ft_lstnew(s->last_cmd->content));
	add_list_and_sort(&(s->env), ft_lstnew(s->oldpwd->content));
	add_list_and_sort(&(s->env), ft_lstnew("SHLVL=1"));
}

void	ft_copy_env(char **env)
{
	int			i;
	t_list		*new;
	t_static	*s;

	s = ft_get_static();
	i = 1;
	new = NULL;
	if (*env == NULL)
	{
		get_default_env(s);
		return ;
	}
	s->env = ft_lstnew(env[0]);
	while (env[i])
	{
		add_list_and_sort(&(s->env), ft_lstnew(env[i]));
		i++;
	}
}

void	print_env_cpy(void)
{
	t_list		*temp;
	char		*value;
	t_static	*s;

	s = ft_get_static();
	temp = s->env;
	value = NULL;
	while (temp)
	{
		value = ft_strchr(temp->content, '=');
		if (ft_strcmp("OLDPWD", temp->content))
			printf("%s\n", (char *)temp->content);
		temp = temp->next;
	}
}

void	ft_env(char **input)
{
	if (input[1] != NULL)
	{
		printf("env: %s: No such file or directory\n", input[1]);
		return ;
	}
	print_env_cpy();
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:05:15 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:02:37 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_digit(char *str)
{
	size_t	i;
	int		sign;

	if (!str)
		return (0);
	i = 0;
	sign = 0;
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i])
	{
		while (str[i] && (str[i] == '+' || str[i] == '-'))
		{
			sign += 1;
			i++;
		}
		while (str[i] && (str[i] >= '0' && str[i] <= '9'))
			i++;
		if (str[i] || sign > 1)
			return (0);
	}
	return (1);
}

void	ft_exit(char **input)
{
	int	i;

	i = 0;
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (input && input[1] == NULL)
		exit_with_no_argc(input);
	if (input[1])
	{
		if (!check_digit(input[1]))
			not_numeric_error(input[1]);
		else if (input[2] != NULL)
		{
			ft_print_error("exit: too many arguments", 1);
			return ;
		}
		else
		{
			i = ft_atoi(input[1]);
			ft_free_exit(input, i);
		}
	}
	if (!ft_get_proccess())
		ft_save_history();
	exit(ft_get_error());
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 18:49:20 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 14:56:28 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_modify(char *str, t_list *node)
{
	char	*new1;
	char	*new2;
	char	*old;

	old = node->content;
	new1 = ft_substr(old, 0, 11 + var_len(str) + 1);
	if (!ft_strchr(old, '='))
		new2 = ft_strjoin(new1, "=\"");
	else
		new2 = ft_strjoin(new1, "\"");
	free(new1);
	new1 = ft_strjoin(str + var_len(str) + 1, "\"");
	node->content = ft_strjoin(new2, new1);
	free(new1);
	free(new2);
}

void	modify_exp(char *str)
{
	t_list		*temp;
	char		*old;
	t_static	*s;

	s = ft_get_static();
	temp = s->exp;
	while (temp)
	{
		old = temp->content;
		if (ft_strncmp(old + 11, str, var_len(str)) == 0 && ((old
					+ 11)[var_len(str)] == '\0' || (old
				+ 11)[var_len(str)] == '='))
			break ;
		temp = temp->next;
	}
	if (temp)
		ft_modify(str, temp);
}

void	modify_env(char *str)
{
	t_list		*temp;
	char		*old;
	t_static	*s;

	s = ft_get_static();
	temp = s->env;
	while (temp)
	{
		old = temp->content;
		if (ft_strncmp(old, str, var_len(str) + 1) == 0)
			break ;
		temp = temp->next;
	}
	if (temp)
		temp->content = str;
}

void	ft_export_var(char *input)
{
	char	*var;
	char	*old;

	var = ft_strdup(input);
	free(input);
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
	int			i;
	t_static	*s;
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utiles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:46:02 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/04 10:13:17 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

 void	leaks()
{
	system("leaks -q minishell");
} 


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
			printf("%p\n", temp->content);
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utiles2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:47:40 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 16:51:27 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	var_len(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

char	*var_existed(char *str)
{
	t_list		*temp;
	char		*var;
	char		*exp;
	t_static	*s;

	s = ft_get_static();
	temp = s->exp;
	if (!str)
		return (NULL);
	var = ft_substr(str, 0, var_len(str));
	while (temp)
	{
		exp = temp->content;
		if (exp && ft_strncmp(exp + 11, var, var_len(str)) == 0 && ((exp
					+ 11)[var_len(str)] == '\0'
			|| (exp + 11)[var_len(str)] == '='))
		{
			free(var);
			return (exp);
		}		
		temp = temp->next;
	}
	free(var);
	return (NULL);
}

char	*add_exp_syntax(char *str)
{
	char	*new1;
	char	*new2;
	char	*new3;

	new1 = ft_substr(str, 0, var_len(str) + 1);
	new2 = ft_strjoin("declare -x ", new1);
	free(new1);
	new1 = ft_strjoin(new2, "\"");
	free(new2);
	new2 = ft_substr(str, var_len(str) + 1, ft_strlen(str) - var_len(str) + 1);
	new3 = ft_strjoin(new2, "\"");
	free(new2);
	new2 = ft_strjoin(new1, new3);
	free(new1);
	free(new3);
	return (new2);
}

void	add_new_var_exp(char *str)
{
	t_list		*new;
	char		*new1;
	char		*new2;
	t_static	*s;

	s = ft_get_static();
	new1 = NULL;
	new2 = NULL;
	if (!str)
		return ;
	if (ft_strchr(str, '=') && ft_strlen(ft_strchr(str, '=')) == 1)
	{
		new1 = ft_strjoin("declare -x ", str);
		new2 = ft_strjoin(new1, "\"\"");
		free(new1);
	}
	else
	{
		if (!ft_strchr(str, '='))
			new2 = ft_strjoin("declare -x ", str);
		else
			new2 = add_exp_syntax(str);
	}
	new = ft_lstnew(new2);
	add_list_and_sort(&(s->exp), new);
}

void	add_new_var_env(char *str)
{
	t_list		*new;
	t_static	*s;

	s = ft_get_static();
	new = ft_lstnew(str);
	add_list_and_sort(&(s->env), new);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:30:13 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 13:52:11 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_put_new_node_first(t_list **list, t_list *new)
{
	int		len;
	char	*str;
	char	*strnew;

	str = (*list)->content;
	strnew = new->content;
	len = ft_var_len(strnew);
	if (ft_strncmp(str, strnew, len) == 0 && str[len] == '='
		&& strnew[len] == '=')
	{
		ft_node_substitute(list, &new);
		return (1);
	}
	else if (ft_strcmp((*list)->content, new->content) > 0)
	{
		ft_lstadd_front(list, new);
		return (1);
	}
	return (0);
}

// len = ft_var_len(insert) + 1; check until '='
// add or substitute and sort

int	ft_substitute_or_insert(t_list **list, t_list *temp, t_list *new)
{
	int		len;
	char	*str;
	char	*strnew;

	str = (*list)->content;
	strnew = new->content;
	len = ft_var_len(strnew);
	if (ft_strncmp(str, strnew, len) == 0 && str[len] == '='
		&& strnew[len] == '=')
	{
		ft_node_substitute(&(temp->next), &new);
		return (1);
	}
	if (temp->next && ft_strcmp(temp->next->content, new->content) > 0)
	{
		ft_front_insert(&temp, &new);
		return (1);
	}
	return (0);
}

void	add_list_and_sort(t_list **list, t_list *new)
{
	t_list	*temp;

	temp = *list;
	if (check_put_new_node_first(list, new) == 1)
		return ;
	while (temp)
	{
		if (ft_substitute_or_insert(list, temp, new))
			break ;
		temp = temp->next;
		if (!temp)
			ft_lstadd_back(list, new);
	}
}

int	ft_count_node(t_list *list)
{
	int		i;
	t_list	*temp;

	i = 0;
	temp = list;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

char	**list_to_matrix(t_list *list)
{
	char	**matrix;
	int		n;
	int		i;
	t_list	*temp;

	i = 0;
	n = ft_count_node(list);
	temp = list;
	matrix = (char **)malloc(sizeof(char *) * (n + 1));
	if (!matrix)
		return (NULL);
	while (i < n)
	{
		matrix[i] = ft_strdup((char *)temp->content);
		temp = temp->next;
		i++;
	}
	matrix[i] = NULL;
	i = 0;
	return (matrix);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utiles.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:49:09 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/04 10:13:46 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_var_len(char *var)
{
	int	i;

	i = 0;
	if (!var)
		return (0);
	while (var[i])
	{
		if (var[i] == '=')
			break ;
		i++;
	}
	return (i);
}

t_list	*ft_locate_node(t_list *list, char *var_name)
{
	t_list	*temp;
	int		len;
	char	*str;

	if (!list)
	{
		printf("this is failing\n");
		return (NULL);
	}
	len = (int)ft_strlen(var_name);
	temp = list;
	str = NULL;
	while (temp)
	{
		str = temp->content;
		if (!ft_strncmp(str, var_name, len) && (str[len] == '='
				|| str[len] == '\0'))
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

void	ft_remove_node(t_list *list, int index)
{
	t_list	*before;
	int		i;
	t_list	*after;
	t_list	*temp;

	i = 0;
	temp = list;
	while (i < index - 1)
	{
		temp = temp->next;
		i++;
	}
	before = temp;
	temp = temp->next;
	after = temp->next;
	before->next = after;
	free(temp);
}

void custom_free(void *data) {
    free(data);
}

void	ft_node_substitute(t_list **old, t_list **new)
{
//	leaks();
	if (*old && *new)
	{
		printf("%s\n", (*old)->content);
		char *aux = ft_strdup((*new)->content);
		// free((*old)->content);
		(*old)->content = aux;
		//if (ft_strnstr(((*old)->content), "PWD=", ft_strlen((*old)->content)))
	
	}
	ft_lstclear(new, custom_free);
//	leaks();
}

void	ft_front_insert(t_list **front_node, t_list **new)
{
	t_list	*back_node;

	back_node = (*front_node)->next;
	(*front_node)->next = *new;
	(*new)->next = back_node;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:03:03 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/26 17:52:14 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_pwd(void)
{
	char	buf[4096];

	printf("%s\n", getcwd(buf, sizeof(buf)));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 18:05:01 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 13:01:31 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_unset_var(char *var)
{
	int			index1;
	int			index2;
	t_static	*s;

	s = ft_get_static();
	index1 = get_var_index_env(var);
	index2 = get_var_index_exp(var);
	if (index1 >= 0)
		unset_var(var, index1, s->env);
	if (index2 >= 0)
		unset_var(var, index2, s->exp);
}

void	ft_unset(char **input)
{
	int	i;

	i = 1;
	if (input[i] == NULL)
		return ;
	while (input[i])
	{
		if (!input[i][0])
		{
			ft_putstr_fd("minishell: unset: `': not a valid identifier\n",
				STDERR_FILENO);
			errno = 1;
			return ;
		}
		if (!is_valid_option(input[i]))
		{
			not_valid_iden_error(input[i]);
			return ;
		}
		ft_unset_var(input[i]);
		i++;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utiles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:51:16 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:01:47 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	get_var_index_exp(char *var_name)
{
	int			len;
	t_list		*temp;
	int			index;
	char		*ref;
	t_static	*s;

	s = ft_get_static();
	index = 0;
	temp = s->exp;
	len = ft_strlen(var_name);
	ref = temp->content + 11;
	while (temp)
	{
		if (ft_strncmp(ref, var_name, len - 1) == 0 && ref[len] == '=')
			break ;
		temp = temp->next;
		if (temp)
			ref = temp->content + 11;
		index++;
	}
	if (!temp)
		return (-1);
	return (index);
}

int	get_var_index_env(char *var_name)
{
	int			len;
	t_list		*temp;
	int			index;
	char		*ref;
	t_static	*s;

	s = ft_get_static();
	index = 0;
	temp = s->env;
	len = ft_strlen(var_name);
	ref = temp->content;
	while (temp)
	{
		if (ft_strncmp(ref, var_name, len - 1) == 0 && ref[len] == '=')
			break ;
		temp = temp->next;
		if (temp)
			ref = temp->content;
		index++;
	}
	if (!temp)
		return (-1);
	return (index);
}

int	is_valid_option(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (!ft_isalpha(str[i]))
		{
			if (!ft_isdigit(str[i]))
			{
				if (str[i] != '_')
					return (0);
			}
		}
		i++;
	}
	return (1);
}

void	unset_var(char *var_name, int index, t_list *list)
{
	t_list		*node_to_remove;
	t_static	*s;

	s = ft_get_static();
	node_to_remove = ft_locate_node(list, var_name);
	ft_remove_node(list, index);
}

void	not_valid_iden_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	errno = 1;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/29 16:40:26 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	executer(char *cmd, t_input *input)
{
	int			cloud[2];
	int			value;
	char		**env;
	t_static	*s;

	s = ft_get_static();
	env = NULL;
	env = list_to_matrix(s->env);
	cloud[0] = dup(0);
	cloud[1] = dup(1);

	value = ft_executer_exec(input, env);
	dup2_with_error_check(cloud[1], 1);
	close(cloud[1]);
	dup2_with_error_check(cloud[0], 0);
	close(cloud[0]);
	ft_free((void **)&cmd);
	ft_free_split_2(&env);
	return (value);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:21:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:21:48 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*cut_input(char *str, int *i)
{
	char	**split;
	char	*result;

	result = NULL;
	split = ft_lexer(str);
	if (!split)
		return (NULL);
	if (*split)
		result = ft_strdup(*split);
	ft_free_split_2(&split);
	if (!result)
	{
		if (i)
			*i = -1;
		return (str);
	}
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_chang_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:26:31 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:26:47 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_change_input(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("inp");
	get_redir(content);
	if (!content->str)
		return (-1);
	fd = open(content->str, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
		return (-1);
	}
	lex->in = fd;
	return (fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_change_out_apd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:28:19 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:28:39 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_change_output_apd(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("apd");
	get_redir(content);
	if (!content->str)
		return (-1);
	fd = open(content->str, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
		return (-1);
	}
	lex->out = fd;
	return (fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_change_out_trc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:27:24 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:27:44 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_change_output_trc(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("trc");
	get_redir(content);
	if (!content->str)
		return (-1);
	fd = open(content->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
		return (-1);
	}
	lex->out = fd;
	return (fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_clear_content.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:57:53 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:13:47 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_exc_clear_content(t_list **result)
{
	ft_lstclear(result, &ft_exc_free_content);
	return (NULL);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_free_content.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:31:02 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 15:49:30 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_exc_free_content(void *cnt_ptr)
{
	t_argument	*content;

	content = (t_argument *)cnt_ptr;
	if (ft_strncmp(content->type, "cmd", 3))
		close(content->fd);
	ft_free((void **)&content->str);
	ft_free((void **)&content->type);
	free(cnt_ptr);
	return ;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_lex_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:59:31 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 17:59:46 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_lex_word(t_list **result, t_exc_lex *lex)
{
	t_list	*tmp;
	char	*wrd;

	wrd = ft_exc_make_word(lex->input + lex->i);
	tmp = NULL;
	tmp = ft_exc_new_node(wrd, lex->status, lex);
	lex->status = non;
	ft_free((void **)&wrd);
	ft_lstadd_back(result, tmp);
	if (!tmp)
		return (-1);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_make_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:02:28 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:14:41 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_exc_make_cmd(t_list **input)
{
	char		*result;
	char		*tmp1;
	t_list		*ptr;
	t_argument	*tmp2;

	result = NULL;
	tmp1 = NULL;
	ptr = *input;
	while (ptr)
	{
		tmp2 = (t_argument *)ptr->content;
		if (!ft_strncmp(tmp2->type, "cmd", 3))
		{
			tmp1 = result;
			result = ft_good_strjoin(result, "   ");
			ft_free((void **)&tmp1);
			tmp1 = result;
			result = ft_good_strjoin(result, tmp2->str);
			ft_free((void **)&tmp1);
		}
		ptr = ptr->next;
	}
	ft_exc_clear_content(input);
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_make_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:02:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:15:31 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_make_redir(char *cmd, t_input *line)
{
	int		std[2];
	t_list	*input;

	std[0] = STDIN_FILENO;
	std[1] = STDOUT_FILENO;
	input = ft_exc_lex_input(cmd, std);
	if (!input)
	{
		if (errno != 9)
			return (errno);
		else
			return (1);
	}
	line->in = dup(std[0]);
	if (std[0] != STDIN_FILENO)
		line->here = 1;
	else
		line->here = 0;
	line->out = dup(std[1]);
	if (line->in == -1 || line->out == -1)
		return (errno);
	line->cmd = ft_exc_make_cmd(&input);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_make_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:59:04 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:16:10 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_exc_make_word(char *input)
{
	int		i;
	int		j;
	char	*str;
	char	*result;
	t_cmd	cmd;

	i = 0;
	j = 0;
	ft_init_cmd(&cmd);
	while (input[i])
	{
		j = ft_check_char(&cmd, input[i]);
		if (!j || j == -1)
			break ;
		i += 1;
	}
	str = ft_strdup(input);
	str[i] = '\0';
	result = ft_strdup(str);
	ft_free((void **)&str);
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_new_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:58:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:29:36 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	*ft_error_to_new_node(void *content)
{
	ft_exc_free_content(content);
	return (NULL);
}

t_list	*ft_exc_new_node(char *argument, t_redir type, t_exc_lex *lex)
{
	t_list		*result;
	t_argument	*content;

	content = malloc(sizeof(t_argument));
	if (!content)
		return (NULL);
	if (argument)
		content->str = ft_strdup(argument);
	else
		content->str = ft_strdup("");
	content->type = NULL;
	if (type == non)
		content->type = ft_strdup("cmd");
	else
		content->fd = ft_exc_open_fd(content, type, lex);
	if (!content->str || !content->type || content->fd == -1)
		return (ft_error_to_new_node((void *)content));
	result = ft_lstnew((void *)content);
	if (!result)
		return (ft_error_to_new_node((void *)content));
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_open_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:30:22 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:30:37 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_open_fd(t_argument *content, t_redir type, t_exc_lex *lex)
{
	if (type == inp)
		return (ft_exc_change_input(content, lex));
	else if (type == hre)
		return (ft_exc_here_doc(content, lex));
	else if (type == trc)
		return (ft_exc_change_output_trc(content, lex));
	else if (type == apd)
		return (ft_exc_change_output_apd(content, lex));
	return (-1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:03:30 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/29 16:41:25 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_executer_exec(t_input *input, char **env)
{
	int	result;


	result = 0;
	dup2_with_error_check(input->in, STDIN_FILENO);
	close(input->in);
	dup2_with_error_check(input->out, STDOUT_FILENO);
	close(input->out);
	result = ft_exc_execution(input->cmd, env);
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:17:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:27:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_check_parse_normal_char(t_exc_lex *lex, int *space)
{
	if (lex->major > 2 || lex->minor > 2 || lex->redirs > 2)
	{
		err_unexpected(lex->minor, lex->major, lex->first);
		return (-1);
	}
	lex->major = 0;
	lex->minor = 0;
	lex->redirs = 0;
	*space = 0;
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_dup_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:18:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:21:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	loop_check_dup(t_exc_lex *lex, char *input, int *space)
{
	lex->j = ft_check_char(&lex->cmd, input[lex->i]);
	if (!lex->j || lex->j == -1)
	{
		if (!lex->j)
		{
			if (lex->major || lex->minor || lex->redirs)
				*space = 1;
		}
		ft_init_cmd(&lex->cmd);
	}
	else
	{
		if (ft_check_parse_normal_char(lex, space) == -1)
			return (-1);
	}
	if (lex->j == -1 && (input[lex->i] == '<' || input[lex->i] == '>'))
	{
		if (ft_check_parse_redirs(lex, space) == -1)
			return (-1);
	}
	return (0);
}

int	ft_check_dup_redir(char *input)
{
	t_exc_lex	lex;
	int			space;

	space = 0;
	lex.input = input;
	ft_init_exc_lex(&lex);
	while (input[lex.i])
	{
		if (loop_check_dup(&lex, input, &space))
			return (-1);
		lex.i += 1;
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:17:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:28:22 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_check_parse_redirs(t_exc_lex *lex, int *space)
{
	if (*space)
		return (err_unexpected(lex->minor, lex->major, 'A'));
	if (lex->input[lex->i] == '<' && !lex->major)
	{
		if (!lex->redirs)
			lex->first = '<';
		lex->minor += 1;
		lex->redirs += 1;
	}
	else if (lex->input[lex->i] == '<' && lex->major)
		return (err_unexpected(lex->minor, lex->major, lex->first));
	if (lex->input[lex->i] == '>' && !lex->minor)
	{
		if (!lex->redirs)
			lex->first = '>';
		lex->major += 1;
		lex->redirs += 1;
	}
	else if (lex->input[lex->i] == '>' && lex->minor)
		return (err_unexpected(lex->minor, lex->major, lex->first));
	if (lex->major > 2 || lex->minor > 2 || lex->redirs > 2)
		return (err_unexpected(lex->minor, lex->major, lex->first));
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_exc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:15:16 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:28:58 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_error_make_list(t_list **result, t_exc_lex *lex, int error)
{
	ft_init_exc_lex(lex);
	ft_exc_clear_content(result);
	if (error)
		err_unexpected(0, 0, 0);
	return (NULL);
}

int	err_unexpected(int minor, int major, char first)
{	
	if (first == 'A')
	{
		if (minor)
			ft_print_error("syntax error near unexpected token `<'", 258);
		if (major)
			ft_print_error("syntax error near unexpected token `>'", 258);
	}
	else if ((minor > 2) || (major == 2 && !minor))
		ft_print_error("syntax error near unexpected token `<'", 258);
	else if ((major > 2) || (minor == 2 && !major))
		ft_print_error("syntax error near unexpected token `>'", 258);
	else if (minor && !major && first == '<')
		ft_print_error("syntax error near unexpected token `>'", 258);
	else if (major && !minor && first == '>')
		ft_print_error("syntax error near unexpected token `<'", 258);
	else
		ft_print_error("syntax error near unexpected token `newline'", 258);
	return (-1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exc_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:14:28 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:30:03 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_execution(char *cmd, char **env)
{
	char	**input;

	if (!cmd)
		return (0);
	input = ft_lexer(cmd);
	ft_free((void **)&cmd);
	if (!*input)
		return ((int)ft_free_split_2(&input) + 258);
	if (!input[0][0])
	{
		ft_print_error(": command not found", 127);
		return (127);
	}
	else
		return (ft_excuter(input, env));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_exc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:15:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:16:12 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_init_exc_lex(t_exc_lex *lex)
{
	lex->i = 0;
	lex->major = 0;
	lex->minor = 0;
	lex->redirs = 0;
	lex->word = 0;
	lex->space = 1;
	lex->first = '\0';
	lex->status = non;
	lex->in = STDIN_FILENO;
	lex->out = STDOUT_FILENO;
	ft_init_cmd(&lex->cmd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_status_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:19:46 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:30:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_put_status_redir(t_exc_lex *lex, char *input)
{
	input = lex->input;
	ft_init_cmd(&lex->cmd);
	lex->word = INACTIVE;
	if (lex->j == -1)
	{
		if (input[lex->i] == '<')
		{
			if (lex->status == non)
				lex->status = inp;
			else
				lex->status = hre;
		}
		else
		{
			if (lex->status == non)
				lex->status = trc;
			else
				lex->status = apd;
		}
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:20:17 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:31:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	ft_strlen2(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:25:47 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:31:35 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	get_redir(t_argument *content)
{
	char	*str;
	int		i;

	i = 0;
	if (content->str)
		str = cut_input(content->str, &i);
	else
		return ;
	if (i)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
		ft_free((void **)&content->str);
		content->str = NULL;
		return ;
	}
	ft_free((void **)&content->str);
	content->str = str;
	return ;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   good_strjoin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:01:56 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:31:49 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_good_strjoin(char *s1, char*s2)
{
	char	*str;
	char	*ret;

	if (!s1 && !s2)
		return (NULL);
	if (!s1 && s2)
		return (ft_strdup(s2));
	if (s1 && !s2)
		return (ft_strdup(s1));
	str = malloc (ft_strlen(s1) + ft_strlen(s2) + 1);
	ret = str;
	if (!str)
		return (NULL);
	while (*s1)
		*str++ = *s1++;
	while (*s2)
	{
		*str = *s2;
		str++;
		s2++;
	}
	*str = '\0';
	return (ret);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:23:40 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:22:10 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ft_here_doc_owo(t_argument *content, int pipes[2])
{
	char	*str;

	flag = HERE;
	content->str = cut_input(content->str, NULL);
	str = readline("> ");
	if (!str)
		exit (0);
	while (1)
	{
		if (!ft_strncmp(content->str, str, ft_strlen2(content->str) + 1))
			exit(1);
		ft_putstr_fd(str, pipes[1]);
		write(pipes[1], "\n", 1);
		free(str);
		str = readline("> ");
		if (!str)
			exit (0);
	}
	free(str);
	exit(0);
}

int	ft_exc_here_doc(t_argument *content, t_exc_lex *lex)
{
	int		pipes[2];
	int		pid;
	int		status;

	if (pipe(pipes))
		exit(errno);
	content->type = ft_strdup("hre");
	pid = fork();
	if (!pid)
		ft_here_doc_owo(content, pipes);
	waitpid(pid, &status, 0);
	flag = PROCCESS;
	if (WEXITSTATUS(status) == 2)
	{
		flag = 3;
		close(pipes[1]);
		close(pipes[0]);
		errno = 1;
		return (-1);
	}
	close(pipes[1]);
	lex->in = pipes[0];
	return (pipes[0]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:01:34 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:32:25 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*ft_exc_lex_input(char *input, int std[2])
{
	t_exc_lex	lex;
	t_list		*result;

	lex.input = input;
	ft_init_exc_lex(&lex);
	if (!input)
	{
		ft_print_error("syntax error near unexpected token `|'", 257);
		return (NULL);
	}
	else if (ft_check_dup_redir(input) == -1)
		return (NULL);
	result = ft_make_list(&lex);
	std[0] = lex.in;
	std[1] = lex.out;
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:00:23 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 16:04:55 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*ft_make_list(t_exc_lex *lex)
{
	t_list		*result;
	char		*input;

	result = NULL;
	input = lex->input;
	while (input[lex->i])
	{
		lex->j = ft_check_char(&lex->cmd, input[lex->i]);
		if (!lex->j || lex->j == -1)
			ft_put_status_redir(lex, NULL);
		else
		{
			if (lex->word == INACTIVE)
			{
				if (ft_exc_lex_word(&result, lex) == -1)
					return (ft_error_make_list(&result, lex, 0));
				lex->word = ACTIVE;
			}
		}
		lex->i += 1;
	}
	if (lex->status)
		return (ft_error_make_list(&result, lex, 1));
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proccess.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:16:47 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/29 16:38:54 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	proccess_pipex(char *cmd)
{
	int	pid;
	int	status;

	flag = PROCCESS;
	pid = fork_with_error_check();
	ft_put_proccess(1);
	if (!pid)
	{
		flag = 4;
		pipex(cmd);
	}
	waitpid(-1, &status, 0);
	ft_put_error(WEXITSTATUS(status));
}

static void	proccess_solo(char *cmd)
{
	int		pid;
	t_input	line;

	pid = ft_exc_make_redir(cmd, &line);
	if (pid)
	{
		ft_put_error(pid);
		return ;
	}
	ft_is_valid_in(STDIN_FILENO, &line);
	ft_put_error(executer(cmd, &line));
}

void	ft_procces_maker(char *cmd, char **env)
{
	char	**input;

	input = ft_lexer(cmd);
	if (input)
	{
		ft_free_split_2(&input);
		if (count_pipes(cmd) > 0)
			proccess_pipex(cmd);
		else
			proccess_solo(cmd);
	}
	else
		ft_free_split_2(&input);
	env = NULL;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:51:45 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:09 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	count_pipes(char *str)
{
	int		j;
	int		i;
	int		n;
	t_cmd	cmd;

	j = 4;
	i = 0;
	n = 0;
	ft_init_cmd(&cmd);
	while (str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == -1)
		{
			if (str[i] == '<' || str[i] == '>')
				ft_init_cmd(&cmd);
			else if (!str[i])
				break ;
			else
				n += 1;
		}
		i++;
	}
	return (n);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:52:17 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:20:36 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_first_child(char *cmd, int pipe[2])
{
	int		pid;
	t_input	input;

	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		return (pipex_error_make_redir(pipe));
	pid = fork_with_error_check();
	if (!pid)
	{
		close(pipe[0]);
		dup2_with_error_check(pipe[1], input.out);
		close(pipe[1]);
		exit(executer(cmd, &input));
	}
	close(pipe[1]);
	return (pipe[0]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:55:32 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:23 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_strndup(const char *s1, size_t n)
{
	char	*str;
	size_t	i;
	size_t	j;

	str = malloc(sizeof(char) * (n + 1));
	if (!str || !s1 || !n)
		return (ft_free((void **)&str));
	i = 0;
	j = 0;
	str[n] = '\0';
	while (i < n && s1[i])
		str[i++] = s1[j++];
	return (str);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_pipex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:56:09 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:37 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_get_cmd_pipex(char **cmd)
{
	size_t	nxt_cmd;
	char	*str;

	nxt_cmd = get_next_index_pipex(*cmd);
	if (nxt_cmd)
		nxt_cmd -= 1;
	str = ft_strndup(*cmd, nxt_cmd);
	*cmd += get_next_index_pipex(*cmd);
	return (str);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_index.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:51:15 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:59 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	get_next_index_pipex(char *str)
{
	int		j;
	int		i;
	int		n;
	t_cmd	cmd;

	j = 4;
	i = 0;
	n = 0;
	ft_init_cmd(&cmd);
	while (str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == -1)
		{
			if (str[i] == '<' || str[i] == '>')
				ft_init_cmd(&cmd);
			else if (!str[i])
				break ;
			else
				return (i + 1);
		}
		i++;
	}
	return (i + 1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:54:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:20:58 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	pipex_error_make_redir(int pipe[2])
{
	if (flag == 3)
		exit(1);
	else
	{
		close (pipe[1]);
		return (pipe[0]);
	}
}

int	ft_last_child(char *cmd, int fd)
{
	int		pipe[2];
	int		pid;
	t_input	input;

	pipe_with_error_check(pipe);
	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		return (pipex_error_make_redir(pipe));
	pid = fork_with_error_check();
	if (!pid)
	{
		close(pipe[1]);
		ft_is_valid_in(fd, &input);
		close(fd);
		exit(executer(cmd, &input));
	}
	close(fd);
	close(pipe[1]);
	ft_wait_all_children(pid);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mid_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:53:29 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:21:23 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_mid_child(char *cmd, int fd)
{
	int		pipe[2];
	int		pid;
	t_input	input;

	pipe_with_error_check(pipe);
	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		return (pipex_error_make_redir(pipe));
	pid = fork_with_error_check();
	if (!pid)
	{
		ft_is_valid_in(fd, &input);
		close(fd);
		dup2_with_error_check(pipe[1], input.out);
		close(pipe[1]);
		exit(executer(cmd, &input));
	}
	close(input.in);
	close(input.out);
	close(fd);
	close(pipe[1]);
	return (pipe[0]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:56:48 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:45:49 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static	void	pipex_init_pipex(t_pipstr *pipex, int *fd, char *cmd)
{
	*fd = 0;
	pipex->i = 1;
	pipex->cmd_cpy = cmd;
	pipex->n = count_pipes(cmd);
	pipex->status = 0;
}

void	pipex(char *cmd)
{
	t_pipstr	pipex;
	int			pid;
	int			fd;
	int			status;

	status = 0;
	pid = fork_with_error_check();
	if (!pid)
	{
		pipex_init_pipex(&pipex, &fd, cmd);
		pipe_with_error_check(pipex.pipes.start_pipe);
		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		fd = ft_first_child(pipex.cmd, pipex.pipes.start_pipe);
		while (pipex.i < pipex.n)
		{
			pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
			fd = ft_mid_child(pipex.cmd, fd);
			pipex.i += 1;
		}
		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		pipex.status = ft_last_child(pipex.cmd, fd);
	}
	waitpid(-1, &status, 0);
	exit (WEXITSTATUS(status));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:52:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:25:19 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_is_valid_in(int fd, t_input *input)
{
	char	**split;
	size_t	i;
	int		pipes[2];

	i = 0;
	split = ft_lexer(input->cmd);
	if (!split)
		return ;
	while (split[i])
		i++;
	ft_free_split_2(&split);
	if (i == 1 && input->here)
	{
		pipe_with_error_check(pipes);
		close(pipes[1]);
		dup2_with_error_check(input->in, pipes[0]);
		return ;
	}
	dup2_with_error_check(fd, input->in);
	return ;
	(void)pipes;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_all_childs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:54:05 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 18:54:20 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_wait_all_children(int pid)
{
	int	id;
	int	status;
	int	final;

	status = 0;
	waitpid(pid, &status, 0);
	final = status;
	while (1)
	{
		id = waitpid(-1, &status, 0);
		if (id < 0)
			break ;
	}
	exit(WEXITSTATUS(final));
	pid = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 16:21:28 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 20:13:01 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_get_old_history(char **env, int *fd)
{
	int		i;
	char	*str;
	char	*str2;
	char	*str3;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		if (!ft_strncmp("HOME=", env[i], 5))
			break ;
		i++;
	}
	if (!env[i])
		return ;
	str = *(env + i);
	str += 5;
	str2 = ft_strjoin(str, "/.minishell_history");
	*fd = open(str2, O_CREAT | O_RDWR | O_APPEND, 0644);
	ft_free((void *)&str2);
	if (*fd < 0)
		return ;
	str3 = get_next_line(*fd);
	while(str3)
	{
		str3[ft_strlen(str3) - 1] = '\0';
		if (str3)
			add_history(str3);
		ft_free((void *)&str3);
		str3 = get_next_line(*fd);
	}
}

int	len_2_fill(int x, int new)
{
	static int	value = 0;

	if (x)
		value = new;
	return (value);
}

void	save_and_clear(void *content)
{
	char		*str;
	static int	i = 0;

	str = (char *)content;
	if (!str)
		return ;
	if (*str && (!((ft_get_history()) == STDIN_FILENO)))
	{
		if (i >= len_2_fill(0, 0))
		{
			ft_putstr_fd(str, ft_get_history());
			ft_putstr_fd("\n", ft_get_history());
		}
	}
	i++;
	ft_free((void **)&str);
	return ;
}

char	*ft_get_history_path(char **env)
{
	size_t	i;
	char	*str;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		if (!ft_strncmp("HOME=", env[i], 5))
			break ;
		i++;
	}
	if (!env[i])
		return (NULL);
	str = *(env + i);
	str += 5;
	return (ft_strjoin(str, "/.minishell_history"));
}

void	ft_save_history_in_list(int fd, t_list **list)
{
	char	*str;

	str = get_next_line(fd);
	while (str)
	{
		str[ft_strlen(str) - 1] = '\0';
		ft_lstadd_back(list, ft_lstnew((void *)ft_strdup(str)));
		ft_free((void *)&str);
		str = get_next_line(fd);
	}
	return ;
}

int	fd_old_history_4_list(char **env, t_save_history *his)
{
	int		fd;
	char	*str2;

	str2 = ft_get_history_path(env);
	if (!str2)
		return (-1);
	fd = open(str2, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
	ft_save_history_in_list(fd, &his->old_history);
	ft_lstadd_back(&his->old_history, his->history);
	close(fd);
	fd = open(str2, O_CREAT | O_RDWR | O_TRUNC, 0644);
	ft_free((void *)&str2);
	return (fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_history.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 19:49:06 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 19:27:23 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_save_history(void)
{
	t_save_history	his;
	
	his.old_history = NULL;
	his.history = (ft_get_static())->history;
	his.fd = ft_get_history();
	if (his.fd != -1 && his.fd != STDIN_FILENO)
		close(his.fd);
	his.env = list_to_matrix((ft_get_static())->env);
	his.fd = fd_old_history_4_list(his.env, &his);
	ft_free_split_2(&(his.env));
	if (his.fd == -1)
		return ;
	his.len = ft_lstsize(his.old_history);
	his.len = his.len - 501;
	if (his.len < 0)
		his.len = 0;
	len_2_fill(1, his.len);
	ft_put_history(his.fd);
	ft_lstclear(&his.old_history, save_and_clear);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   holaaaa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:11:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:26:38 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

char	**ft_lexer(char *str)
{
	char			**result;
	int				len;

	if (ft_check_argument(str) == -1)
		return (NULL);
	len = ft_lexer_len_n_arguments(str);
	result = ft_calloc(sizeof(char *), (len + 1));
	if (!result)
		return (NULL);
	result[len] = NULL;
	ft_alloc_parse_result(&result, str, len);
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:05:02 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:57:48 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	count_arguments(char *str)
{
	int				i;
	int				command;
	int				n_commands;

	if (!str)
		return (0);
	i = 0;
	n_commands = 0;
	command = 0;
	while (str[i] == ' ')
		i++;
	if (str[i])
		n_commands = 1;
	while (str[i])
	{
		while (str[i] != ' ' && str[i])
			i++;
		command = 0;
		while (str[i] == ' ' && str[i])
			i++;
		if (str[i] && str[i] != ' ' && !command)
			command += n_commands++;
		i++;
	}
	return (n_commands);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_delimiter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:06:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:06:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	dollar_delimiter(char c)
{
	if (!c || c == '$' || c == '\'' || c == '\"' \
		|| c == '|' || c == '<' || c == '>')
		return (0);
	return (1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_argument.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:12:10 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:12:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//			ft_check_argument
//		return values:
//		1 if is a valid argument 
//		0 if is a empty argument
//		-1 if is a invalid argument

int	ft_check_argument(char *str)
{
	int	len;

	len = ft_get_next_command(str);
	if (len == -4)
		return (0);
	while (len > 0)
	{
		str += len;
		len = ft_get_next_command(str);
	}
	if (len == -1)
		return (-1);
	return (1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:37:30 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:52:04 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_delimiters(t_cmd *cmd, char c);

int	ft_check_char(t_cmd *cmd, char c)
{
	if (c == '\'' || c == '\"')
		return (ft_lex_quotes(cmd, c));
	else if (c == '$')
		return (ft_lex_dollar(cmd, c));
	else if (c == '|' || c == '<' || c == '>')
		return (ft_lex_delimiters(cmd, c));
	else if (!c)
		return (-1);
	else if (c == ' ')
		return (ft_lex_space(cmd, c));
	else if (c == '?')
		return (ft_lex_interrogation(cmd, c));
	else
		return (ft_lex_chars(cmd, c));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dollar_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:52:37 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:20:27 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ft_dollar_fill_init(t_dollar_fill *strc, int *x, char *dst)
{
	strc->j = 4;
	strc->i = 0;
	strc->z = 0;
	strc->x = x;
	strc->dst = dst;
	strc->tmp = (ft_get_static())->env;
}

static int	ft_dollar_fill_util(t_dollar_fill *strc)
{
	if (!strc->tmp)
	{
		if (strc->str2[0] == '?')
		{
			strc->str3 = ft_itoa(ft_get_error());
			ft_strlcpy(strc->dst, strc->str3, ft_strlen(strc->str3) + 1);
			strc->i = ft_strlen(strc->str3);
			strc->x[0] += strc->i;
			ft_free((void **)&strc->str3);
			ft_free((void **)&strc->str2);
			return (0);
		}
		else
			strc->dst[0] = '\0';
		ft_free((void **)&strc->str2);
		return (0);
	}
	return (1);
}

static int	ft_dollar_fill_util_2(t_dollar_fill *strc, char *str)
{
	if (strc->i == 1)
	{
		strc->dst[0] = '$';
		strc->x[0] += 1;
		return (0);
	}
	strc->str2 = malloc(sizeof(char) * (strc->i + 1));
	if (!strc->str2)
		return (0);
	ft_strlcpy(strc->str2, str, strc->i);
	strc->str2[strc->i] = '\0';
	return (1);
}

static int	ft_dollar_checkin(t_dollar_fill *strc, char *str, t_cmd	*cmd)
{
	if (!str)
		return (0);
	if (!str[0])
	{
		strc->dst[0] = '$';
		strc->x[0] += 1;
		return (0);
	}
	strc->j = ft_check_char(cmd, str[strc->i++]);
	return (1);
}

void	ft_dollar_fill(char *str, t_cmd cmd, int *x, char *dst)
{
	t_dollar_fill	strc;

	ft_dollar_fill_init(&strc, x, dst);
	if (!ft_dollar_checkin(&strc, str, &cmd))
		return ;
	while (strc.j == 3)
		strc.j = ft_check_char(&cmd, str[strc.i++]);
	if (!ft_dollar_fill_util_2(&strc, str))
		return ;
	while (strc.tmp)
	{
		if (!ft_strncmp((char *)strc.tmp->content, \
		strc.str2, ft_strlen(strc.str2)))
			break ;
		strc.tmp = strc.tmp->next;
	}
	if (!ft_dollar_fill_util(&strc))
		return ;
	strc.str3 = (char *)strc.tmp->content + strc.i;
	ft_strlcpy(dst, strc.str3, ft_strlen(strc.str3) + 1);
	*x += ft_strlen(strc.str3);
	return ;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dollar_len.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:50:05 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 16:43:26 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	dollar_return(t_dollar_len *dollar, int n)
{
	dollar->return_value = n;
	return (-1);
}

static void	init_init_dollar_len(char *str, t_cmd cmd, \
t_dollar_len *dollar)
{
	dollar->j = 4;
	dollar->i = 0;
	dollar->cmd = cmd;
	dollar->str = str;
	dollar->j = ft_check_char(&dollar->cmd, dollar->str[dollar->i]);
	dollar->i += 1;
	dollar->return_value = 0;
	dollar->tmp = (ft_get_static())->env;
	dollar->str2 = NULL;
}

static int	init_dollar_len(char *str, t_cmd cmd, t_dollar_len *dollar)
{
	if (!str)
		return (dollar_return(dollar, 0));
	if (!str[0])
		return (dollar_return(dollar, 1));
	init_init_dollar_len(str, cmd, dollar);
	while (dollar->j == 3)
	{
		dollar->j = ft_check_char(&dollar->cmd, dollar->str[dollar->i]);
		dollar->i += 1;
	}
	if (dollar->i == 1)
		return (dollar_return(dollar, 1));
	dollar->str2 = malloc(sizeof(char) * (dollar->i + 1));
	if (!dollar->str2)
		return (dollar_return(dollar, 0));
	ft_strlcpy(dollar->str2, dollar->str, dollar->i);
	dollar->str2[dollar->i] = '\0';
	return (0);
}

static int	dollar_flags(t_dollar_len *dollar)
{
	if (dollar->str2[0] == '?')
	{
		ft_free((void **)&dollar->str2);
		dollar->str2 = ft_itoa(ft_get_error());
		dollar->i = ft_strlen(dollar->str2);
		ft_free((void **)&dollar->str2);
		return (dollar->i);
	}
	ft_free((void **)&dollar->str2);
	return (1);
}

int	ft_dollar_len(char *str, t_cmd cmd)
{
	t_dollar_len	dollar;

	if (init_dollar_len(str, cmd, &dollar) == -1)
		return (dollar.return_value);
	while (dollar.tmp)
	{
		if (!ft_strncmp((char *)dollar.tmp->content, \
		dollar.str2, ft_strlen(dollar.str2)))
			break ;
		dollar.tmp = dollar.tmp->next;
	}
	if (!dollar.tmp)
		return (dollar_flags(&dollar));
	ft_free((void **)&dollar.str2);
	return (ft_strlen(((char *)dollar.tmp->content + dollar.i)) + 1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:14:38 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:21:02 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_free_split_2(char ***split)
{
	int	i;

	i = 0;
	while (split[0][i])
	{
		if (split[0][i])
			ft_free((void *)&split[0][i]);
		i++;
	}
	ft_free((void *)&split[0]);
	*split = NULL;
	return (NULL);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:10:48 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 18:06:07 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	errors_get_next_cmd(int x, t_get_next_command *cmd, int n);
static int	init_get_next_command(char *str, t_get_next_command *cmd);
static int	get_next_return_util(t_get_next_command *cmd, int x);
static int	loop_get_next_cmd(t_get_next_command *cmd);
int			ft_get_next_command(char *str);

/*
			ft_get_next_command
		return values:
		-4 if the command is empty
		-3 if is the last or only command
		-2 if is one pipe open
		-1 if the quotes is no closed
		(other value) index of the next command in the array
*/
int	ft_get_next_command(char *str)
{
	t_get_next_command	cmd;

	if (init_get_next_command(str, &cmd) == -1)
		return (cmd.return_status);
	while (cmd.str[cmd.i] && !cmd.end)
	{
		while (cmd.status.status == q_open && cmd.str[cmd.i])
		{
			if (loop_get_next_cmd(&cmd))
				break ;
		}
		cmd.n_commands += 1;
		while (cmd.str[cmd.i] == ' ' && cmd.str[cmd.i] && !cmd.end)
			cmd.i += 1;
		if ((str[cmd.i] == '|' || str[cmd.i] == '<' || str[cmd.i] == '>'))
			return (get_next_return_util(&cmd, 1));
		if (cmd.str[cmd.i] && !cmd.end)
		{
			if ((str[cmd.i] == '|' || str[cmd.i] == '<' || str[cmd.i] == '>'))
				break ;
			cmd.status.status = q_open;
		}
	}
	return (get_next_return_util(&cmd, 0));
}

static int	errors_get_next_cmd(int x, t_get_next_command *cmd, int n)
{
	if (x == 1)
		ft_print_error("syntax error near unexpected token `|'", 258);
	else if (x == 2)
		ft_print_error("syntax error unclosed quotes", 69);
	cmd->return_status = n;
	return (-1);
}

static int	init_get_next_command(char *str, t_get_next_command *cmd)
{
	if (!str)
		return (errors_get_next_cmd(0, cmd, -1));
	cmd->str = str;
	cmd->status.dollar = funtional;
	cmd->status.simple_q = q_close;
	cmd->status.double_q = q_close;
	cmd->status.dollar = funtional;
	cmd->i = 0;
	cmd->n_commands = 0;
	cmd->command = 0;
	cmd->end = 0;
	while (cmd->str[cmd->i] == ' ')
		cmd->i += 1;
	if (cmd->str[cmd->i] == '|')
		return (errors_get_next_cmd(1, cmd, -1));
	if (str[cmd->i])
		cmd->status.status = q_open;
	else
		return (errors_get_next_cmd(0, cmd, -4));
	return (0);
}

static int	get_next_return_util(t_get_next_command *cmd, int x)
{
	if (x)
	{
		if (!cmd->str[cmd->i + 1])
			return (-2);
		else
			return (cmd->i + 1);
	}
	else
	{
		if (!cmd->str[cmd->i] && cmd->status.simple_q == q_close && \
		cmd->status.double_q == q_close)
			cmd->status.status = q_close;
		if (cmd->status.status == q_open)
			return (errors_get_next_cmd(2, cmd, -1));
		return (-3);
	}
}

static int	loop_get_next_cmd(t_get_next_command *cmd)
{
	if (ft_lexer_check_status(&cmd->status, cmd->str, &cmd->i))
	{
		cmd->end = 1;
		return (1);
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:48:30 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:49:08 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_init_cmd(t_cmd *cmd)
{
	cmd->quotes = no_q;
	cmd->dollar_status = q_close;
	cmd->status = q_close;
	cmd->spaces = 1;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_chars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:34:23 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:20:36 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_chars(t_cmd *cmd, char c)
{
	if (cmd->status == q_close)
		cmd->status = q_open;
	if (cmd->dollar_status == q_open)
	{
		cmd->spaces = 1;
		return (3);
	}
	return (2);
	c = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_delimiters.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:17:59 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:18:23 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_delimiters(t_cmd *cmd, char c)
{
	cmd->dollar_status = q_close;
	if (cmd->status == q_close)
		return (-1);
	else
	{
		if (cmd->quotes == no_q)
		{
			cmd->status = q_close;
			return (-1);
		}
		else
			return (2);
	}
	c = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:35:21 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:21:33 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_dollar(t_cmd *cmd, char c)
{
	if (cmd->quotes != s_q)
	{
		if (cmd->spaces)
		{
			cmd->dollar_status = q_open;
			cmd->spaces = 0;
			return (4);
		}
		else
		{
			cmd->dollar_status = q_close;
			cmd->spaces = 3;
			return (3);
		}
	}
	else
	{
		cmd->dollar_status = q_close;
		cmd->spaces = 0;
		return (2);
	}
	c = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_interrogation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:36:11 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:36:29 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_interrogation(t_cmd *cmd, char c)
{
	if (cmd->dollar_status == q_open)
	{
		cmd->dollar_status = q_close;
		if (cmd->spaces)
		{
			cmd->spaces = 3;
			return (2);
		}
		else
		{
			cmd->spaces = 3;
			return (3);
		}
	}
	else
		return (2);
	c = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:16:27 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:37:33 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	lex_quotes_status_close(t_cmd *cmd, char c);
static int	lex_quotes_status_open_noq(t_cmd *cmd, char c);
static int	lex_quotes_status_open_sq(t_cmd *cmd, char c);
int			ft_lex_quotes(t_cmd *cmd, char c);

//	1 no printable, 
//	2 printable, 
//	0 space and -1 finish or special char
int	ft_lex_quotes(t_cmd *cmd, char c)
{
	if (cmd->status == q_close)
		return (lex_quotes_status_close(cmd, c));
	else
	{
		if (cmd->quotes == no_q)
			return (lex_quotes_status_open_noq(cmd, c));
		else
			return (lex_quotes_status_open_sq(cmd, c));
	}
}

static int	lex_quotes_status_close(t_cmd *cmd, char c)
{
	cmd->status = q_open;
	if (c == '\'')
	{
		cmd->dollar_status = q_close;
		cmd->quotes = s_q;
		cmd->spaces = 3;
	}
	else
		cmd->quotes = d_q;
	return (1);
}

static int	lex_quotes_status_open_noq(t_cmd *cmd, char c)
{
	if (c == '\'')
	{
		cmd->quotes = s_q;
		cmd->dollar_status = q_close;
		cmd->spaces = 3;
	}
	else
	{
		cmd->dollar_status = q_close;
		cmd->quotes = d_q;
		cmd->spaces = 3;
	}
	return (1);
}

static int	lex_quotes_status_open_sq(t_cmd *cmd, char c)
{
	if (cmd->quotes == s_q)
	{
		if (c == '\'')
		{
			cmd->dollar_status = q_close;
			cmd->quotes = no_q;
			cmd->spaces = 3;
			return (1);
		}
		else
			return (2);
	}
	else
	{
		if (c == '\"')
		{
			cmd->quotes = no_q;
			cmd->dollar_status = q_close;
			cmd->spaces = 3;
			return (1);
		}
		else
			return (2);
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lex_space.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:22:29 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:23:13 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lex_space(t_cmd *cmd, char c)
{
	if (cmd->status == q_close)
		return (0);
	else
	{
		if (cmd->quotes == no_q)
		{
			cmd->status = q_close;
			cmd->dollar_status = q_close;
			cmd->dollar = funtional;
			return (0);
		}
		else
		{
			if (cmd->quotes == d_q)
			{
				cmd->dollar_status = q_close;
				return (2);
			}
			else
				return (2);
		}
	}
	c = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_check_status.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:08:16 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 18:20:02 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	lex_open_simple_quotes(t_command *cmd)
{
	cmd->simple_q = q_open;
	cmd->status = q_open;
}

static void	lex_open_double_quotes(t_command *cmd)
{
	cmd->double_q = q_open;
	cmd->status = q_open;
}

static void	ch_lexer_are_quotes(t_command *cmd, char *str, int *i)
{
	if (cmd->simple_q == q_open || cmd->double_q == q_open)
	{
		if (str[*i] == '\'' && cmd->simple_q == q_open)
		{
			if (!str[*i + 1] || str[*i + 1] == ' ')
				cmd->status = q_close;
			cmd->simple_q = q_close;
			cmd->dollar = funtional;
		}
		else if (str[*i] == '\"' && cmd->double_q == q_open)
		{
			if (!str[*i + 1] || str[*i + 1] == ' ')
				cmd->status = q_close;
			cmd->double_q = q_close;
		}
	}
	else if (str[*i] == '\'' && cmd->simple_q == q_close \
	&& cmd->simple_q == q_close)
		lex_open_simple_quotes(cmd);
	else if (str[*i] == '\"' && cmd->simple_q == q_close && \
	cmd->simple_q == q_close)
		lex_open_double_quotes(cmd);
}

int	ft_lexer_check_status(t_command *cmd, char *str, int *i)
{
	if (cmd->status == q_open)
	{
		if (cmd->simple_q == q_close && cmd->double_q == q_close \
		&& (str[*i] == '|' || str[*i] == '<' || str[*i] == '>'))
		{
			cmd->status = q_close;
			return (1);
		}
		else if (str[*i] == '\'' || str[*i] == '\"')
			ch_lexer_are_quotes(cmd, str, i);
		else if ((str[*i] == ' ' || !str[*i]) && \
		cmd->simple_q == q_close && cmd->double_q == q_close)
			cmd->status = q_close;
		else if (str[*i] == '$' && cmd->simple_q == q_close)
		{
			if (str[*i + 1] == '$')
				*i += 1;
			while (!str[*i + 1] && dollar_delimiter(str[*i + 1]))
				*i += 1;
		}
		*i += 1;
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_len.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:56:10 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/25 11:21:30 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	init_lexer_len(t_lexer_len *len)
{
	len->len = 0;
	len->i = 0;
	len->j = 0;
	len->x = 0;
	ft_init_cmd(&len->cmd);
}

int	ft_lexer_len_argument(char *str)
{
	t_lexer_len	len;

	init_lexer_len(&len);
	while (!len.j && str[len.i])
	{
		len.j = ft_check_char(&len.cmd, str[len.i]);
		if (len.j == 2)
			len.len += 1;
		if (len.j == 4)
			len.len += ft_dollar_len(str + len.i + 1, len.cmd);
		len.i += 1;
	}
	if (str[len.i] && len.j >= 0)
	{
		while (str[len.i] && len.j > 0)
		{
			len.j = ft_check_char(&len.cmd, str[len.i]);
			if (len.j == 2)
				len.len += 1;
			if (len.j == 4)
				len.len += ft_dollar_len(str + len.i + 1, len.cmd);
			len.i += 1;
		}
	}
	return (len.len);
}

void	ft_lexer_len_loop(t_lexer_len *len, char *str)
{
	len->j = ft_check_char(&len->cmd, str[len->i]);
	if (len->j > 0 && len->x != 1)
	{
		len->len += 1;
		len->x = 1;
	}
	len->i += 1;
}

int	ft_lexer_len_n_arguments(char *str)
{
	t_lexer_len	len;

	init_lexer_len(&len);
	while (str[len.i] && !len.j && len.j != -1)
	{
		len.j = ft_check_char(&len.cmd, str[len.i]);
		if (len.j > 0)
		{
			len.len += 1;
			len.x = 1;
		}
		len.i += 1;
	}
	while (str[len.i] && len.j >= 0)
	{
		while (str[len.i] && len.j > 0 && len.j != -1)
			ft_lexer_len_loop(&len, str);
		len.x = 0;
		ft_init_cmd(&len.cmd);
		while (str[len.i] && !len.j && len.j != -1)
			ft_lexer_len_loop(&len, str);
	}
	return (len.len);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:54:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/25 11:27:01 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_lexer_fill_str(char *str, char **str2)
{
	t_lexer_len	l;

	init_lexer_len(&l);
	while (!l.j && str[l.i])
	{
		l.j = ft_check_char(&l.cmd, str[l.i]);
		if (l.j == 2)
			str2[0][l.x++] = str[l.i];
		if (l.j == 4)
			ft_dollar_fill(str + l.i + 1, l.cmd, &l.x, str2[0] + l.x);
		l.i += 1;
	}
	if (str[l.i] && l.j >= 0)
	{
		while (str[l.i] && l.j > 0)
		{
			l.j = ft_check_char(&l.cmd, str[l.i]);
			if (l.j == 2)
				str2[0][l.x++] = str[l.i];
			if (l.j == 4)
				ft_dollar_fill(str + l.i + 1, l.cmd, &l.x, str2[0] + l.x);
			l.i += 1;
		}
	}
}
// debugg string: printf("[%d] = '%c', j = %d\n", i, str[i], j);

int	ft_lexer_get_next_argument(char *str)
{
	t_cmd			cmd;
	int				len;
	int				i;
	int				j;

	len = 0;
	i = 0;
	j = 0;
	ft_init_cmd(&cmd);
	while (str[i] && !j)
		j = ft_check_char(&cmd, str[i++]);
	while (str[i] && j >= 0)
	{
		while (str[i] && j > 0)
			j = ft_check_char(&cmd, str[i++]);
		return (i);
	}
	return (ft_strlen(str));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 14:58:27 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/25 11:38:09 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	init_alloc_parse(char ***res, t_alloc_parse *all)
{
	all->i = 0;
	all->x = 0;
	all->arg_len = 0;
	all->result = *res;
}

//	this funtion count how many numbers need per argument
void	ft_alloc_parse_result(char ***result_ptr, char *str, int len)
{
	t_alloc_parse	all;

	init_alloc_parse(result_ptr, &all);
	while (len)
	{
		all.arg_len = ft_lexer_len_argument(str);
		all.str2 = ft_calloc(sizeof(char), all.arg_len + 1);
		if (!all.str2)
			return ;
		ft_lexer_fill_str(str, &all.str2);
		all.str2[all.arg_len] = '\0';
		if (*all.str2)
			all.result[all.x++] = all.str2;
		else
		{
			if (*str != '$')
				all.result[all.x++] = all.str2;
			else
				ft_free((void **)&all.str2);
		}
		all.i = ft_lexer_get_next_argument(str);
		str += all.i;
		len--;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:09:21 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 14:38:37 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**split_path(char **env)
{
	char	*all_path;
	char	**path;
	int		i;

	i = 0;
	all_path = NULL;
	while (env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
			all_path = ft_strdup(env[i] + 5);
		i++;
	}
	if (all_path == NULL)
		return (NULL);
	path = ft_split(all_path, ':');
	free(all_path);
	return (path);
}

void	*ft_free_split(void **strings)
{
	int	i;

	i = 0;
	while (strings && strings[i])
	{
		free(strings[i]);
		i++;
	}
	free(strings);
	return (NULL);
}

char	*get_cmd(char *argv)
{
	char	*cmd;

	cmd = ft_strdup(argv);
	return (cmd);
}
char	*get_full_path_util(char **path_list, char *path, char *temp, char *cmd)
{
	if (path_list)
	{
		ft_free_split_2((char ***)&path_list);
		exit_cmd_not_found(path, temp, cmd);
		return (NULL);
	}
	else
	{
		char *script = ft_strjoin("./", cmd);
		free(cmd);
		return (script);
	}	
}

char	*get_full_path(char **env, char *cmd)
{
	char	**path_list;
	int		i;
	char	*path;
	char	*temp;

	i = 0;
	path = NULL;
	temp = NULL;
	path_list = split_path(env);
	while (path_list && path_list[i])
	{
		temp = ft_strjoin(path_list[i++], "/");
		path = ft_strjoin(temp, cmd);
		if (access(path, X_OK) == 0)
		{
			free(temp);
			if (path_list)
				ft_free_split_2((char ***)&path_list);
			return (path);
		}
		ft_free((void **)&temp);
		ft_free((void **)&path);
	}
	return (get_full_path_util(path_list, path, temp, cmd));
}

char	*cmd_path(char *argv, char **env)
{
	char	*cmd;

	cmd = get_cmd(argv);
	if (ft_strncmp("/", cmd, 1) == 0 || ft_strncmp("./", cmd, 2) == 0
		|| ft_strncmp("../", cmd, 3) == 0)
		return (cmd);
	return (get_full_path(env, cmd));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 12:21:27 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/02 17:50:49 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	pipe_with_error_check(int *fd)
{
	if (pipe(fd) == -1)
	{
		perror("Pipe");
		exit(EXIT_FAILURE);
	}
}

void	dup2_with_error_check(int fd1, int fd2)
{
	if (dup2(fd1, fd2) == -1)
	{
		perror("Dup2");
		exit(EXIT_FAILURE);
	}
}

int	open_with_error_check(const char *path, int oflag)
{
	int	fd;

	fd = open(path, oflag, 0666);
	if (fd == -1)
	{
		perror(path);
		exit(EXIT_FAILURE);
	}
	return (fd);
}

int	fork_with_error_check(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	exit_cmd_not_found(char *path, char *temp, char *cmd)
{
	free(path);
	free(temp);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	errno = 127;
	exit(127);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:10:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/12 17:20:02 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>
#include <lexer.h>

void	wait_all_children(void)
{
	int	id;
	int	status;

	status = 0;
	while (1)
	{
		id = waitpid(-1, &status, 0);
		if (id < 0)
			break ;
	}
	exit(status);
}

void	child_execute_preparation(int i, int *end, int argc, char **argv)
{
	int	outfile_fd;

	outfile_fd = 0;
	if (i < argc - 2)
	{
		dup2_with_error_check(end[WRITE], STDOUT_FILENO);
		close(end[WRITE]);
	}
	if (i == argc - 2)
	{
		if (ft_strcmp(argv[1], "here_doc") == 0)
			outfile_fd = open_with_error_check(argv[argc - 1],
					O_WRONLY | O_CREAT | O_APPEND);
		else
			outfile_fd = open_with_error_check(argv[argc - 1],
					O_WRONLY | O_CREAT | O_TRUNC);
		dup2_with_error_check(outfile_fd, STDOUT_FILENO);
	}
}

int	execve_with_error_check(char **argv, char **env)
{
	char	*path;
	char	**arguments;

	arguments = argv;
	if (!arguments)
		exit(0);
	path = cmd_path(*argv, env);
	if (execve(path, argv, env) == -1)
	{
		perror(*argv);
		exit(127);
	}
	exit (0);
	return (0);
}

int	execute_cmd(t_var5 var)
{
	int	end[2];
	int	pid;

	if (var.i < var.argc - 2)
		pipe_with_error_check(end);
	pid = fork_with_error_check();
	if (pid == 0)
	{
		if (var.fd == -1)
			exit(1);
		dup2_with_error_check(var.fd, STDIN_FILENO);
		close(var.fd);
		child_execute_preparation(var.i, end, var.argc, var.argv);
		execve_with_error_check(&var.argv[var.i], var.env);
	}
	if (var.i == var.argc - 2)
	{
		close(end[READ]);
		wait_all_children();
	}
	if (var.fd)
		close(var.fd);
	close(end[WRITE]);
	var.fd = end[READ];
	return (var.fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:07:01 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/03 15:29:28 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	print_error(char *str)
{
	ft_putendl_fd(str, 2);
	exit(1);
}

void	here_doc(char *limiter, int fd)
{
	char	*s;
	char	*limit;

	limit = ft_strjoin(limiter, "\n");
	while (1)
	{
		ft_putstr_fd("here_doc > ", 1);
		s = get_next_line(STDIN_FILENO);
		if (!s)
			exit(1);
		if (ft_strcmp(s, limit) == 0)
		{
			free(s);
			free(limit);
			break ;
		}
		ft_putstr_fd(s, fd);
		free(s);
	}
}

t_var2	get_input_fd_for_cmd1(char **argv)
{
	int		end[2];
	t_var2	var;

	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		var.i = 3;
		pipe_with_error_check(end);
		here_doc(argv[2], end[WRITE]);
		close(end[WRITE]);
		var.fd = end[READ];
	}
	else
	{
		var.i = 2;
		var.fd = open(argv[1], O_RDONLY);
		if (var.fd == -1)
			perror(argv[1]);
	}
	return (var);
}

/*
int	main(int argc, char **argv, char **env)
{
	int		temp;
	t_var5	var5;
	t_var2	var2;

	if (argc < 5)
		print_error("Not enough arguments.");
	var2 = get_input_fd_for_cmd1(argv);
	var5.argc = argc;
	var5.argv = argv;
	var5.env = env;
	var5.i = var2.i;
	var5.fd = var2.fd;
	while (var5.i < var5.argc - 1)
	{	
		temp = execute_cmd(var5);
		var5.i++;
		var5.fd = temp;
	}
	return (0);
}
*/
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:05:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 15:32:25 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_samu.h"

char	*ft_free_samu(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
	return (NULL);
}

char	*ft_error(struct s_data_base *file)
{
	file->saved_line_len = 0;
	ft_free_samu(&file->saved_line);
	return (NULL);
}

int	ft_verify_new_line(struct s_data_base *file)
{
	int	i;

	i = 0;
	if (!file->saved_line)
		return (0);
	while (i != file->saved_line_len)
	{
		if (file->saved_line[i] == '\n' || \
		file->saved_line[i] == '\r')
			return (1);
		++i;
	}
	return (0);
}

char	*ft_read_line(struct s_data_base *file, int *read_number, int fd)
{
	char	*str_for_realloc;

	str_for_realloc = NULL;
	*read_number = read(fd, file->buffer_line, BUFFER_SIZE);
	if (*read_number < 0)
		return (ft_error(file));
	str_for_realloc = ft_realloc(file->saved_line, file->buffer_line, \
	file->saved_line_len, *read_number);
	file->saved_line_len = file->saved_line_len + *read_number;
	ft_free_samu(&file->saved_line);
	file->saved_line = str_for_realloc;
	return (file->saved_line);
}

char	*get_next_line_samu(int fd)
{
	static struct s_data_base	file[FOPEN_MAX];
	int							read_number;
	char						*result;

	read_number = 1;
	if (read(fd, 0, 0) < 0 || fd < 0 \
	|| BUFFER_SIZE < 1 || fd > FOPEN_MAX)
		return (ft_error(&file[fd]));
	while (read_number)
	{
		if (ft_verify_new_line(&file[fd]))
			return (ft_split_lines(&file[fd]));
		if (!ft_read_line(&file[fd], &read_number, fd))
			return (0);
	}
	result = ft_mem_cpy(file[fd].saved_line, file[fd].saved_line_len);
	ft_error(&file[fd]);
	return (result);
}
/*
int	main(int argc, char **argv)
{
	char	*str;
	int		fd;
	int		i = 4;

	fd = open(argv[1], O_RDONLY);
	while (i)
	{
		str = get_next_line(fd);
		printf("The line result:%s", str);
		write(1, "el resultado", 12);
		write(1, str, 100);
		--i;
	}
	//system("leaks -q aoeu");
	return (0);
}
*/
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:16:06 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/31 14:35:58 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line_samu.h"

char	*ft_split_lines(struct s_data_base *file)
{
	int		i;
	char	*string_for_db;
	char	*string_result;

	i = 0;
	while (file->saved_line[i] != '\n' && file->saved_line[i] != '\r')
		++i;
	string_result = ft_mem_cpy(file->saved_line, i + 1);
	string_for_db = ft_mem_cpy((file->saved_line + (i + 1)), \
	(file->saved_line_len - (i + 1)));
	file->saved_line_len = file->saved_line_len - (i + 1);
	ft_free_samu(&file->saved_line);
	file->saved_line = string_for_db;
	return (string_result);
}

char	*ft_mem_cpy(char *str, int len)
{
	char	*result;
	int		i;

	i = 0;
	result = NULL;
	result = malloc(sizeof(char) * ((int)len + 1));
	if (!result || !str || len <= 0)
	{
		if (result)
			ft_free_samu(&result);
		return (NULL);
	}
	while (i <= len)
		result[i++] = '\0';
	i = 0;
	while (i != len)
		result[i++] = *str++;
	return (result);
}

char	*ft_realloc(char *str1, char *str2, int str1_len, int str2_len)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!str1 || !str2)
	{
		if (str1)
			return (ft_mem_cpy(str1, str1_len));
		if (str2)
			return (ft_mem_cpy(str2, str2_len));
		return (NULL);
	}
	result = malloc(sizeof(char) * ((int)(str1_len + str2_len + 1)));
	if (!result)
		return (NULL);
	result[(str1_len + str2_len)] = '\0';
	while (i != str1_len)
		result[i++] = *str1++;
	while (j != str2_len)
		result[i++] = str2[j++];
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 16:53:23 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 16:42:33 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
void ft_pwd(void)
{
	char buf[4096];

	printf("%s\n", getcwd(buf, sizeof(buf)));
}

int check_digit(char *str)
{
	size_t	i;
	int		sign;

	if (!str)
		return (0);
	i = 0;
	sign = 0;
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i])
	{
		while (str[i] && (str[i] == '+' || str[i] == '-'))
		{
			sign += 1;
			i++;
		}
		while (str[i] && (str[i] >= '0' && str[i] <= '9'))
			i++;
		if (str[i] || sign > 1)
			return (0);
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
	{
		if (!ft_get_proccess())
			ft_save_history();
		ft_free_exit(input, ft_get_error());
	}
	if (input[1])
	{
		if(!check_digit(input[1])) 
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd("exit: ", STDERR_FILENO);
			ft_putstr_fd(input[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument requiered\n", STDERR_FILENO);
			ft_put_error(255);
			if (!ft_get_proccess())
				ft_save_history();
			exit (255);
		}
		else if (input[2] != NULL)
		{
			ft_print_error("exit: too many arguments", 1);
			return ;
		}
		else
		{
			i = ft_atoi(input[1]);
			ft_free_exit(input, i);
		}	
	}
	if (!ft_get_proccess())
		ft_save_history();
	exit (ft_get_error());
}
*/
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
	int		pid;
	int		status;

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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utiles.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:06:21 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 12:52:56 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_free_exit(char **input, int n)
{
	ft_free_input(input);
	exit (n);
}

void	exit_with_no_argc(char **input)
{
	if (!ft_get_proccess())
		ft_save_history();
	ft_free_exit(input, ft_get_error());
}

void	not_numeric_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": numeric argument requiered\n", STDERR_FILENO);
	ft_put_error(255);
	if (!ft_get_proccess())
		ft_save_history();
	exit (255);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:49:41 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 16:43:36 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_cd(char *path)
{
	DIR			*dir;
	t_static	*s;

	s = ft_get_static();
	if (!path)
	{
		if (!ft_get_var("HOME"))
		{
			ft_err_msg("minishell: cd: HOME not set\n", NULL, NULL);
			return ;
		}
		ft_go_somewhere(s, ft_get_var("HOME"));
		return ;
	}
	dir = opendir(path);
	if (dir)
	{
		if (access(path, X_OK) == -1)
			ft_err_msg("minishell: cd: ", path, ": Permission denied\n");
		else
			ft_go_somewhere(s, path);
		closedir(dir);
	}
	else
		ft_err_msg("minishell: cd: ", path, "No such file or directory\n");
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utiles.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:44:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 16:45:40 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_get_var(char *var)
{
	t_static	*s;
	char		*env_var;
	size_t		len;
	t_list		*temp;

	s = ft_get_static();
	len = ft_strlen(var);
	temp = s->env;
	while (temp)
	{
		env_var = temp->content;
		if (env_var && var && !ft_strncmp(env_var, var, len)
			&& env_var[ft_strlen(var)] == '=')
			return (env_var + len + 1);
		temp = temp->next;
	}
	return (NULL);
}

void	update_dir_for_exp(t_static *s)
{
	t_list	*oldpwd;
	t_list	*old;
	t_list	*new;
	t_list	*new_oldpwd;
	char	buf[4096];

	old = NULL;
	new = ft_lstnew(ft_strjoin("declare -x PWD=", getcwd(buf, sizeof(buf))));
	old = ft_locate_node(s->exp, "declare -x PWD");
	new_oldpwd = ft_lstnew(ft_strjoin("declare -x OLD", old->content + 11));
	oldpwd = ft_locate_node(s->exp, "declare -x OLDPWD");
	ft_node_substitute(&oldpwd, &new_oldpwd);
	ft_node_substitute(&old, &new);
}

void	update_dir_for_env(t_static *s)
{
	t_list	*oldpwd;
	t_list	*old;
	t_list	*new;
	t_list	*new_oldpwd;
	char	buf[4096];

	old = NULL;
	new = ft_lstnew(ft_strjoin("PWD=", getcwd(buf, sizeof(buf))));
	old = ft_locate_node(s->env, "PWD");
	new_oldpwd = ft_lstnew(ft_strjoin("OLD", old->content));
	oldpwd = ft_locate_node(s->env, "OLDPWD");
	ft_node_substitute(&oldpwd, &new_oldpwd);
	ft_node_substitute(&old, &new);  
}

void	ft_go_somewhere(t_static *s, char *somewhere)
{
	chdir(somewhere);
	update_dir_for_env(s);
	update_dir_for_exp(s);
}

void	ft_err_msg(char *s1, char *path, char *s2)
{
	ft_putstr_fd(s1, STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(s2, STDERR_FILENO);
	errno = 1;
	ft_put_error(1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:51:34 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:05:23 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_only_n(char *str)
{
	int	i;

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
		return (1);
	return (0);
}

int	ft_echo_no_argv(char **input)
{
	if (input[1] == NULL)
	{
		printf("\n");
		return (1);
	}
	return (0);
}

void	ft_print_holder(char **input, int i)
{
	if (ft_strcmp(input[1], "-n") == 0)
		printf("%s", input[i]);
	else
		printf("%s\n", input[i]);
}

void	ft_echo(char **input)
{
	int	i;

	i = 1;
	if (ft_echo_no_argv(input))
		return ;
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
	ft_print_holder(input, i);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:04:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 15:30:33 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	get_default_env(t_static *s)
{
	char	buf[4096];

	s->env = ft_lstnew(ft_strjoin("PWD=", getcwd(buf, sizeof(buf))));
	add_list_and_sort(&(s->env), ft_lstnew(s->last_cmd->content));
	add_list_and_sort(&(s->env), ft_lstnew(s->oldpwd->content));
	add_list_and_sort(&(s->env), ft_lstnew("SHLVL=1"));
}

void	ft_copy_env(char **env)
{
	int			i;
	t_list		*new;
	t_static	*s;

	s = ft_get_static();
	i = 1;
	new = NULL;
	if (*env == NULL)
	{
		get_default_env(s);
		return ;
	}
	s->env = ft_lstnew(env[0]);
	while (env[i])
	{
		add_list_and_sort(&(s->env), ft_lstnew(env[i]));
		i++;
	}
}

void	print_env_cpy(void)
{
	t_list		*temp;
	char		*value;
	t_static	*s;

	s = ft_get_static();
	temp = s->env;
	value = NULL;
	while (temp)
	{
		value = ft_strchr(temp->content, '=');
		if (ft_strcmp("OLDPWD", temp->content))
			printf("%s\n", (char *)temp->content);
		temp = temp->next;
	}
}

void	ft_env(char **input)
{
	if (input[1] != NULL)
	{
		printf("env: %s: No such file or directory\n", input[1]);
		return ;
	}
	print_env_cpy();
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:05:15 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:02:37 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_digit(char *str)
{
	size_t	i;
	int		sign;

	if (!str)
		return (0);
	i = 0;
	sign = 0;
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i])
	{
		while (str[i] && (str[i] == '+' || str[i] == '-'))
		{
			sign += 1;
			i++;
		}
		while (str[i] && (str[i] >= '0' && str[i] <= '9'))
			i++;
		if (str[i] || sign > 1)
			return (0);
	}
	return (1);
}

void	ft_exit(char **input)
{
	int	i;

	i = 0;
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (input && input[1] == NULL)
		exit_with_no_argc(input);
	if (input[1])
	{
		if (!check_digit(input[1]))
			not_numeric_error(input[1]);
		else if (input[2] != NULL)
		{
			ft_print_error("exit: too many arguments", 1);
			return ;
		}
		else
		{
			i = ft_atoi(input[1]);
			ft_free_exit(input, i);
		}
	}
	if (!ft_get_proccess())
		ft_save_history();
	exit(ft_get_error());
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 18:49:20 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 14:56:28 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_modify(char *str, t_list *node)
{
	char	*new1;
	char	*new2;
	char	*old;

	old = node->content;
	new1 = ft_substr(old, 0, 11 + var_len(str) + 1);
	if (!ft_strchr(old, '='))
		new2 = ft_strjoin(new1, "=\"");
	else
		new2 = ft_strjoin(new1, "\"");
	free(new1);
	new1 = ft_strjoin(str + var_len(str) + 1, "\"");
	node->content = ft_strjoin(new2, new1);
	free(new1);
	free(new2);
}

void	modify_exp(char *str)
{
	t_list		*temp;
	char		*old;
	t_static	*s;

	s = ft_get_static();
	temp = s->exp;
	while (temp)
	{
		old = temp->content;
		if (ft_strncmp(old + 11, str, var_len(str)) == 0 && ((old
					+ 11)[var_len(str)] == '\0' || (old
				+ 11)[var_len(str)] == '='))
			break ;
		temp = temp->next;
	}
	if (temp)
		ft_modify(str, temp);
}

void	modify_env(char *str)
{
	t_list		*temp;
	char		*old;
	t_static	*s;

	s = ft_get_static();
	temp = s->env;
	while (temp)
	{
		old = temp->content;
		if (ft_strncmp(old, str, var_len(str) + 1) == 0)
			break ;
		temp = temp->next;
	}
	if (temp)
		temp->content = str;
}

void	ft_export_var(char *input)
{
	char	*var;
	char	*old;

	var = ft_strdup(input);
	free(input);
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
	int			i;
	t_static	*s;
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utiles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:46:02 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/04 10:13:17 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

 void	leaks()
{
	system("leaks -q minishell");
} 


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
			printf("%p\n", temp->content);
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utiles2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:47:40 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 16:51:27 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	var_len(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

char	*var_existed(char *str)
{
	t_list		*temp;
	char		*var;
	char		*exp;
	t_static	*s;

	s = ft_get_static();
	temp = s->exp;
	if (!str)
		return (NULL);
	var = ft_substr(str, 0, var_len(str));
	while (temp)
	{
		exp = temp->content;
		if (exp && ft_strncmp(exp + 11, var, var_len(str)) == 0 && ((exp
					+ 11)[var_len(str)] == '\0'
			|| (exp + 11)[var_len(str)] == '='))
		{
			free(var);
			return (exp);
		}		
		temp = temp->next;
	}
	free(var);
	return (NULL);
}

char	*add_exp_syntax(char *str)
{
	char	*new1;
	char	*new2;
	char	*new3;

	new1 = ft_substr(str, 0, var_len(str) + 1);
	new2 = ft_strjoin("declare -x ", new1);
	free(new1);
	new1 = ft_strjoin(new2, "\"");
	free(new2);
	new2 = ft_substr(str, var_len(str) + 1, ft_strlen(str) - var_len(str) + 1);
	new3 = ft_strjoin(new2, "\"");
	free(new2);
	new2 = ft_strjoin(new1, new3);
	free(new1);
	free(new3);
	return (new2);
}

void	add_new_var_exp(char *str)
{
	t_list		*new;
	char		*new1;
	char		*new2;
	t_static	*s;

	s = ft_get_static();
	new1 = NULL;
	new2 = NULL;
	if (!str)
		return ;
	if (ft_strchr(str, '=') && ft_strlen(ft_strchr(str, '=')) == 1)
	{
		new1 = ft_strjoin("declare -x ", str);
		new2 = ft_strjoin(new1, "\"\"");
		free(new1);
	}
	else
	{
		if (!ft_strchr(str, '='))
			new2 = ft_strjoin("declare -x ", str);
		else
			new2 = add_exp_syntax(str);
	}
	new = ft_lstnew(new2);
	add_list_and_sort(&(s->exp), new);
}

void	add_new_var_env(char *str)
{
	t_list		*new;
	t_static	*s;

	s = ft_get_static();
	new = ft_lstnew(str);
	add_list_and_sort(&(s->env), new);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:30:13 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/29 13:52:11 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	check_put_new_node_first(t_list **list, t_list *new)
{
	int		len;
	char	*str;
	char	*strnew;

	str = (*list)->content;
	strnew = new->content;
	len = ft_var_len(strnew);
	if (ft_strncmp(str, strnew, len) == 0 && str[len] == '='
		&& strnew[len] == '=')
	{
		ft_node_substitute(list, &new);
		return (1);
	}
	else if (ft_strcmp((*list)->content, new->content) > 0)
	{
		ft_lstadd_front(list, new);
		return (1);
	}
	return (0);
}

// len = ft_var_len(insert) + 1; check until '='
// add or substitute and sort

int	ft_substitute_or_insert(t_list **list, t_list *temp, t_list *new)
{
	int		len;
	char	*str;
	char	*strnew;

	str = (*list)->content;
	strnew = new->content;
	len = ft_var_len(strnew);
	if (ft_strncmp(str, strnew, len) == 0 && str[len] == '='
		&& strnew[len] == '=')
	{
		ft_node_substitute(&(temp->next), &new);
		return (1);
	}
	if (temp->next && ft_strcmp(temp->next->content, new->content) > 0)
	{
		ft_front_insert(&temp, &new);
		return (1);
	}
	return (0);
}

void	add_list_and_sort(t_list **list, t_list *new)
{
	t_list	*temp;

	temp = *list;
	if (check_put_new_node_first(list, new) == 1)
		return ;
	while (temp)
	{
		if (ft_substitute_or_insert(list, temp, new))
			break ;
		temp = temp->next;
		if (!temp)
			ft_lstadd_back(list, new);
	}
}

int	ft_count_node(t_list *list)
{
	int		i;
	t_list	*temp;

	i = 0;
	temp = list;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

char	**list_to_matrix(t_list *list)
{
	char	**matrix;
	int		n;
	int		i;
	t_list	*temp;

	i = 0;
	n = ft_count_node(list);
	temp = list;
	matrix = (char **)malloc(sizeof(char *) * (n + 1));
	if (!matrix)
		return (NULL);
	while (i < n)
	{
		matrix[i] = ft_strdup((char *)temp->content);
		temp = temp->next;
		i++;
	}
	matrix[i] = NULL;
	i = 0;
	return (matrix);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utiles.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:49:09 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/04 10:13:46 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_var_len(char *var)
{
	int	i;

	i = 0;
	if (!var)
		return (0);
	while (var[i])
	{
		if (var[i] == '=')
			break ;
		i++;
	}
	return (i);
}

t_list	*ft_locate_node(t_list *list, char *var_name)
{
	t_list	*temp;
	int		len;
	char	*str;

	if (!list)
	{
		printf("this is failing\n");
		return (NULL);
	}
	len = (int)ft_strlen(var_name);
	temp = list;
	str = NULL;
	while (temp)
	{
		str = temp->content;
		if (!ft_strncmp(str, var_name, len) && (str[len] == '='
				|| str[len] == '\0'))
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

void	ft_remove_node(t_list *list, int index)
{
	t_list	*before;
	int		i;
	t_list	*after;
	t_list	*temp;

	i = 0;
	temp = list;
	while (i < index - 1)
	{
		temp = temp->next;
		i++;
	}
	before = temp;
	temp = temp->next;
	after = temp->next;
	before->next = after;
	free(temp);
}

void custom_free(void *data) {
    free(data);
}

void	ft_node_substitute(t_list **old, t_list **new)
{
//	leaks();
	if (*old && *new)
	{
		printf("%s\n", (*old)->content);
		char *aux = ft_strdup((*new)->content);
		// free((*old)->content);
		(*old)->content = aux;
		//if (ft_strnstr(((*old)->content), "PWD=", ft_strlen((*old)->content)))
	
	}
	ft_lstclear(new, custom_free);
//	leaks();
}

void	ft_front_insert(t_list **front_node, t_list **new)
{
	t_list	*back_node;

	back_node = (*front_node)->next;
	(*front_node)->next = *new;
	(*new)->next = back_node;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:03:03 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/26 17:52:14 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_pwd(void)
{
	char	buf[4096];

	printf("%s\n", getcwd(buf, sizeof(buf)));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 18:05:01 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 13:01:31 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_unset_var(char *var)
{
	int			index1;
	int			index2;
	t_static	*s;

	s = ft_get_static();
	index1 = get_var_index_env(var);
	index2 = get_var_index_exp(var);
	if (index1 >= 0)
		unset_var(var, index1, s->env);
	if (index2 >= 0)
		unset_var(var, index2, s->exp);
}

void	ft_unset(char **input)
{
	int	i;

	i = 1;
	if (input[i] == NULL)
		return ;
	while (input[i])
	{
		if (!input[i][0])
		{
			ft_putstr_fd("minishell: unset: `': not a valid identifier\n",
				STDERR_FILENO);
			errno = 1;
			return ;
		}
		if (!is_valid_option(input[i]))
		{
			not_valid_iden_error(input[i]);
			return ;
		}
		ft_unset_var(input[i]);
		i++;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utiles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:51:16 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:01:47 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	get_var_index_exp(char *var_name)
{
	int			len;
	t_list		*temp;
	int			index;
	char		*ref;
	t_static	*s;

	s = ft_get_static();
	index = 0;
	temp = s->exp;
	len = ft_strlen(var_name);
	ref = temp->content + 11;
	while (temp)
	{
		if (ft_strncmp(ref, var_name, len - 1) == 0 && ref[len] == '=')
			break ;
		temp = temp->next;
		if (temp)
			ref = temp->content + 11;
		index++;
	}
	if (!temp)
		return (-1);
	return (index);
}

int	get_var_index_env(char *var_name)
{
	int			len;
	t_list		*temp;
	int			index;
	char		*ref;
	t_static	*s;

	s = ft_get_static();
	index = 0;
	temp = s->env;
	len = ft_strlen(var_name);
	ref = temp->content;
	while (temp)
	{
		if (ft_strncmp(ref, var_name, len - 1) == 0 && ref[len] == '=')
			break ;
		temp = temp->next;
		if (temp)
			ref = temp->content;
		index++;
	}
	if (!temp)
		return (-1);
	return (index);
}

int	is_valid_option(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (!ft_isalpha(str[i]))
		{
			if (!ft_isdigit(str[i]))
			{
				if (str[i] != '_')
					return (0);
			}
		}
		i++;
	}
	return (1);
}

void	unset_var(char *var_name, int index, t_list *list)
{
	t_list		*node_to_remove;
	t_static	*s;

	s = ft_get_static();
	node_to_remove = ft_locate_node(list, var_name);
	ft_remove_node(list, index);
}

void	not_valid_iden_error(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	errno = 1;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/29 16:40:26 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	executer(char *cmd, t_input *input)
{
	int			cloud[2];
	int			value;
	char		**env;
	t_static	*s;

	s = ft_get_static();
	env = NULL;
	env = list_to_matrix(s->env);
	cloud[0] = dup(0);
	cloud[1] = dup(1);

	value = ft_executer_exec(input, env);
	dup2_with_error_check(cloud[1], 1);
	close(cloud[1]);
	dup2_with_error_check(cloud[0], 0);
	close(cloud[0]);
	ft_free((void **)&cmd);
	ft_free_split_2(&env);
	return (value);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:21:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:21:48 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*cut_input(char *str, int *i)
{
	char	**split;
	char	*result;

	result = NULL;
	split = ft_lexer(str);
	if (!split)
		return (NULL);
	if (*split)
		result = ft_strdup(*split);
	ft_free_split_2(&split);
	if (!result)
	{
		if (i)
			*i = -1;
		return (str);
	}
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_chang_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:26:31 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:26:47 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_change_input(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("inp");
	get_redir(content);
	if (!content->str)
		return (-1);
	fd = open(content->str, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
		return (-1);
	}
	lex->in = fd;
	return (fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_change_out_apd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:28:19 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:28:39 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_change_output_apd(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("apd");
	get_redir(content);
	if (!content->str)
		return (-1);
	fd = open(content->str, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
		return (-1);
	}
	lex->out = fd;
	return (fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_change_out_trc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:27:24 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:27:44 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_change_output_trc(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("trc");
	get_redir(content);
	if (!content->str)
		return (-1);
	fd = open(content->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
		return (-1);
	}
	lex->out = fd;
	return (fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_clear_content.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:57:53 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:13:47 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_exc_clear_content(t_list **result)
{
	ft_lstclear(result, &ft_exc_free_content);
	return (NULL);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_free_content.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:31:02 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 15:49:30 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_exc_free_content(void *cnt_ptr)
{
	t_argument	*content;

	content = (t_argument *)cnt_ptr;
	if (ft_strncmp(content->type, "cmd", 3))
		close(content->fd);
	ft_free((void **)&content->str);
	ft_free((void **)&content->type);
	free(cnt_ptr);
	return ;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_lex_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:59:31 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 17:59:46 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_lex_word(t_list **result, t_exc_lex *lex)
{
	t_list	*tmp;
	char	*wrd;

	wrd = ft_exc_make_word(lex->input + lex->i);
	tmp = NULL;
	tmp = ft_exc_new_node(wrd, lex->status, lex);
	lex->status = non;
	ft_free((void **)&wrd);
	ft_lstadd_back(result, tmp);
	if (!tmp)
		return (-1);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_make_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:02:28 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:14:41 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_exc_make_cmd(t_list **input)
{
	char		*result;
	char		*tmp1;
	t_list		*ptr;
	t_argument	*tmp2;

	result = NULL;
	tmp1 = NULL;
	ptr = *input;
	while (ptr)
	{
		tmp2 = (t_argument *)ptr->content;
		if (!ft_strncmp(tmp2->type, "cmd", 3))
		{
			tmp1 = result;
			result = ft_good_strjoin(result, "   ");
			ft_free((void **)&tmp1);
			tmp1 = result;
			result = ft_good_strjoin(result, tmp2->str);
			ft_free((void **)&tmp1);
		}
		ptr = ptr->next;
	}
	ft_exc_clear_content(input);
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_make_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:02:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:15:31 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_make_redir(char *cmd, t_input *line)
{
	int		std[2];
	t_list	*input;

	std[0] = STDIN_FILENO;
	std[1] = STDOUT_FILENO;
	input = ft_exc_lex_input(cmd, std);
	if (!input)
	{
		if (errno != 9)
			return (errno);
		else
			return (1);
	}
	line->in = dup(std[0]);
	if (std[0] != STDIN_FILENO)
		line->here = 1;
	else
		line->here = 0;
	line->out = dup(std[1]);
	if (line->in == -1 || line->out == -1)
		return (errno);
	line->cmd = ft_exc_make_cmd(&input);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_make_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:59:04 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:16:10 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_exc_make_word(char *input)
{
	int		i;
	int		j;
	char	*str;
	char	*result;
	t_cmd	cmd;

	i = 0;
	j = 0;
	ft_init_cmd(&cmd);
	while (input[i])
	{
		j = ft_check_char(&cmd, input[i]);
		if (!j || j == -1)
			break ;
		i += 1;
	}
	str = ft_strdup(input);
	str[i] = '\0';
	result = ft_strdup(str);
	ft_free((void **)&str);
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_new_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:58:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:29:36 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	*ft_error_to_new_node(void *content)
{
	ft_exc_free_content(content);
	return (NULL);
}

t_list	*ft_exc_new_node(char *argument, t_redir type, t_exc_lex *lex)
{
	t_list		*result;
	t_argument	*content;

	content = malloc(sizeof(t_argument));
	if (!content)
		return (NULL);
	if (argument)
		content->str = ft_strdup(argument);
	else
		content->str = ft_strdup("");
	content->type = NULL;
	if (type == non)
		content->type = ft_strdup("cmd");
	else
		content->fd = ft_exc_open_fd(content, type, lex);
	if (!content->str || !content->type || content->fd == -1)
		return (ft_error_to_new_node((void *)content));
	result = ft_lstnew((void *)content);
	if (!result)
		return (ft_error_to_new_node((void *)content));
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_open_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:30:22 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:30:37 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_open_fd(t_argument *content, t_redir type, t_exc_lex *lex)
{
	if (type == inp)
		return (ft_exc_change_input(content, lex));
	else if (type == hre)
		return (ft_exc_here_doc(content, lex));
	else if (type == trc)
		return (ft_exc_change_output_trc(content, lex));
	else if (type == apd)
		return (ft_exc_change_output_apd(content, lex));
	return (-1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:03:30 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/29 16:41:25 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_executer_exec(t_input *input, char **env)
{
	int	result;


	result = 0;
	dup2_with_error_check(input->in, STDIN_FILENO);
	close(input->in);
	dup2_with_error_check(input->out, STDOUT_FILENO);
	close(input->out);
	result = ft_exc_execution(input->cmd, env);
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:17:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:27:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_check_parse_normal_char(t_exc_lex *lex, int *space)
{
	if (lex->major > 2 || lex->minor > 2 || lex->redirs > 2)
	{
		err_unexpected(lex->minor, lex->major, lex->first);
		return (-1);
	}
	lex->major = 0;
	lex->minor = 0;
	lex->redirs = 0;
	*space = 0;
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_dup_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:18:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:21:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	loop_check_dup(t_exc_lex *lex, char *input, int *space)
{
	lex->j = ft_check_char(&lex->cmd, input[lex->i]);
	if (!lex->j || lex->j == -1)
	{
		if (!lex->j)
		{
			if (lex->major || lex->minor || lex->redirs)
				*space = 1;
		}
		ft_init_cmd(&lex->cmd);
	}
	else
	{
		if (ft_check_parse_normal_char(lex, space) == -1)
			return (-1);
	}
	if (lex->j == -1 && (input[lex->i] == '<' || input[lex->i] == '>'))
	{
		if (ft_check_parse_redirs(lex, space) == -1)
			return (-1);
	}
	return (0);
}

int	ft_check_dup_redir(char *input)
{
	t_exc_lex	lex;
	int			space;

	space = 0;
	lex.input = input;
	ft_init_exc_lex(&lex);
	while (input[lex.i])
	{
		if (loop_check_dup(&lex, input, &space))
			return (-1);
		lex.i += 1;
	}
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:17:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:28:22 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_check_parse_redirs(t_exc_lex *lex, int *space)
{
	if (*space)
		return (err_unexpected(lex->minor, lex->major, 'A'));
	if (lex->input[lex->i] == '<' && !lex->major)
	{
		if (!lex->redirs)
			lex->first = '<';
		lex->minor += 1;
		lex->redirs += 1;
	}
	else if (lex->input[lex->i] == '<' && lex->major)
		return (err_unexpected(lex->minor, lex->major, lex->first));
	if (lex->input[lex->i] == '>' && !lex->minor)
	{
		if (!lex->redirs)
			lex->first = '>';
		lex->major += 1;
		lex->redirs += 1;
	}
	else if (lex->input[lex->i] == '>' && lex->minor)
		return (err_unexpected(lex->minor, lex->major, lex->first));
	if (lex->major > 2 || lex->minor > 2 || lex->redirs > 2)
		return (err_unexpected(lex->minor, lex->major, lex->first));
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_exc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:15:16 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:28:58 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_error_make_list(t_list **result, t_exc_lex *lex, int error)
{
	ft_init_exc_lex(lex);
	ft_exc_clear_content(result);
	if (error)
		err_unexpected(0, 0, 0);
	return (NULL);
}

int	err_unexpected(int minor, int major, char first)
{	
	if (first == 'A')
	{
		if (minor)
			ft_print_error("syntax error near unexpected token `<'", 258);
		if (major)
			ft_print_error("syntax error near unexpected token `>'", 258);
	}
	else if ((minor > 2) || (major == 2 && !minor))
		ft_print_error("syntax error near unexpected token `<'", 258);
	else if ((major > 2) || (minor == 2 && !major))
		ft_print_error("syntax error near unexpected token `>'", 258);
	else if (minor && !major && first == '<')
		ft_print_error("syntax error near unexpected token `>'", 258);
	else if (major && !minor && first == '>')
		ft_print_error("syntax error near unexpected token `<'", 258);
	else
		ft_print_error("syntax error near unexpected token `newline'", 258);
	return (-1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exc_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:14:28 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:30:03 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exc_execution(char *cmd, char **env)
{
	char	**input;

	if (!cmd)
		return (0);
	input = ft_lexer(cmd);
	ft_free((void **)&cmd);
	if (!*input)
		return ((int)ft_free_split_2(&input) + 258);
	if (!input[0][0])
	{
		ft_print_error(": command not found", 127);
		return (127);
	}
	else
		return (ft_excuter(input, env));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_exc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:15:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:16:12 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_init_exc_lex(t_exc_lex *lex)
{
	lex->i = 0;
	lex->major = 0;
	lex->minor = 0;
	lex->redirs = 0;
	lex->word = 0;
	lex->space = 1;
	lex->first = '\0';
	lex->status = non;
	lex->in = STDIN_FILENO;
	lex->out = STDOUT_FILENO;
	ft_init_cmd(&lex->cmd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_status_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:19:46 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:30:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_put_status_redir(t_exc_lex *lex, char *input)
{
	input = lex->input;
	ft_init_cmd(&lex->cmd);
	lex->word = INACTIVE;
	if (lex->j == -1)
	{
		if (input[lex->i] == '<')
		{
			if (lex->status == non)
				lex->status = inp;
			else
				lex->status = hre;
		}
		else
		{
			if (lex->status == non)
				lex->status = trc;
			else
				lex->status = apd;
		}
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:20:17 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:31:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	ft_strlen2(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:25:47 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:31:35 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	get_redir(t_argument *content)
{
	char	*str;
	int		i;

	i = 0;
	if (content->str)
		str = cut_input(content->str, &i);
	else
		return ;
	if (i)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(content->str, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
		ft_free((void **)&content->str);
		content->str = NULL;
		return ;
	}
	ft_free((void **)&content->str);
	content->str = str;
	return ;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   good_strjoin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:01:56 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:31:49 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_good_strjoin(char *s1, char*s2)
{
	char	*str;
	char	*ret;

	if (!s1 && !s2)
		return (NULL);
	if (!s1 && s2)
		return (ft_strdup(s2));
	if (s1 && !s2)
		return (ft_strdup(s1));
	str = malloc (ft_strlen(s1) + ft_strlen(s2) + 1);
	ret = str;
	if (!str)
		return (NULL);
	while (*s1)
		*str++ = *s1++;
	while (*s2)
	{
		*str = *s2;
		str++;
		s2++;
	}
	*str = '\0';
	return (ret);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:23:40 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:22:10 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ft_here_doc_owo(t_argument *content, int pipes[2])
{
	char	*str;

	flag = HERE;
	content->str = cut_input(content->str, NULL);
	str = readline("> ");
	if (!str)
		exit (0);
	while (1)
	{
		if (!ft_strncmp(content->str, str, ft_strlen2(content->str) + 1))
			exit(1);
		ft_putstr_fd(str, pipes[1]);
		write(pipes[1], "\n", 1);
		free(str);
		str = readline("> ");
		if (!str)
			exit (0);
	}
	free(str);
	exit(0);
}

int	ft_exc_here_doc(t_argument *content, t_exc_lex *lex)
{
	int		pipes[2];
	int		pid;
	int		status;

	if (pipe(pipes))
		exit(errno);
	content->type = ft_strdup("hre");
	pid = fork();
	if (!pid)
		ft_here_doc_owo(content, pipes);
	waitpid(pid, &status, 0);
	flag = PROCCESS;
	if (WEXITSTATUS(status) == 2)
	{
		flag = 3;
		close(pipes[1]);
		close(pipes[0]);
		errno = 1;
		return (-1);
	}
	close(pipes[1]);
	lex->in = pipes[0];
	return (pipes[0]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:01:34 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 15:32:25 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*ft_exc_lex_input(char *input, int std[2])
{
	t_exc_lex	lex;
	t_list		*result;

	lex.input = input;
	ft_init_exc_lex(&lex);
	if (!input)
	{
		ft_print_error("syntax error near unexpected token `|'", 257);
		return (NULL);
	}
	else if (ft_check_dup_redir(input) == -1)
		return (NULL);
	result = ft_make_list(&lex);
	std[0] = lex.in;
	std[1] = lex.out;
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:00:23 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 16:04:55 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*ft_make_list(t_exc_lex *lex)
{
	t_list		*result;
	char		*input;

	result = NULL;
	input = lex->input;
	while (input[lex->i])
	{
		lex->j = ft_check_char(&lex->cmd, input[lex->i]);
		if (!lex->j || lex->j == -1)
			ft_put_status_redir(lex, NULL);
		else
		{
			if (lex->word == INACTIVE)
			{
				if (ft_exc_lex_word(&result, lex) == -1)
					return (ft_error_make_list(&result, lex, 0));
				lex->word = ACTIVE;
			}
		}
		lex->i += 1;
	}
	if (lex->status)
		return (ft_error_make_list(&result, lex, 1));
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proccess.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:16:47 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/29 16:38:54 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	proccess_pipex(char *cmd)
{
	int	pid;
	int	status;

	flag = PROCCESS;
	pid = fork_with_error_check();
	ft_put_proccess(1);
	if (!pid)
	{
		flag = 4;
		pipex(cmd);
	}
	waitpid(-1, &status, 0);
	ft_put_error(WEXITSTATUS(status));
}

static void	proccess_solo(char *cmd)
{
	int		pid;
	t_input	line;

	pid = ft_exc_make_redir(cmd, &line);
	if (pid)
	{
		ft_put_error(pid);
		return ;
	}
	ft_is_valid_in(STDIN_FILENO, &line);
	ft_put_error(executer(cmd, &line));
}

void	ft_procces_maker(char *cmd, char **env)
{
	char	**input;

	input = ft_lexer(cmd);
	if (input)
	{
		ft_free_split_2(&input);
		if (count_pipes(cmd) > 0)
			proccess_pipex(cmd);
		else
			proccess_solo(cmd);
	}
	else
		ft_free_split_2(&input);
	env = NULL;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:51:45 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:09 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	count_pipes(char *str)
{
	int		j;
	int		i;
	int		n;
	t_cmd	cmd;

	j = 4;
	i = 0;
	n = 0;
	ft_init_cmd(&cmd);
	while (str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == -1)
		{
			if (str[i] == '<' || str[i] == '>')
				ft_init_cmd(&cmd);
			else if (!str[i])
				break ;
			else
				n += 1;
		}
		i++;
	}
	return (n);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:52:17 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:20:36 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_first_child(char *cmd, int pipe[2])
{
	int		pid;
	t_input	input;

	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		return (pipex_error_make_redir(pipe));
	pid = fork_with_error_check();
	if (!pid)
	{
		close(pipe[0]);
		dup2_with_error_check(pipe[1], input.out);
		close(pipe[1]);
		exit(executer(cmd, &input));
	}
	close(pipe[1]);
	return (pipe[0]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:55:32 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:23 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_strndup(const char *s1, size_t n)
{
	char	*str;
	size_t	i;
	size_t	j;

	str = malloc(sizeof(char) * (n + 1));
	if (!str || !s1 || !n)
		return (ft_free((void **)&str));
	i = 0;
	j = 0;
	str[n] = '\0';
	while (i < n && s1[i])
		str[i++] = s1[j++];
	return (str);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_pipex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:56:09 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:37 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_get_cmd_pipex(char **cmd)
{
	size_t	nxt_cmd;
	char	*str;

	nxt_cmd = get_next_index_pipex(*cmd);
	if (nxt_cmd)
		nxt_cmd -= 1;
	str = ft_strndup(*cmd, nxt_cmd);
	*cmd += get_next_index_pipex(*cmd);
	return (str);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_index.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:51:15 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:24:59 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	get_next_index_pipex(char *str)
{
	int		j;
	int		i;
	int		n;
	t_cmd	cmd;

	j = 4;
	i = 0;
	n = 0;
	ft_init_cmd(&cmd);
	while (str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == -1)
		{
			if (str[i] == '<' || str[i] == '>')
				ft_init_cmd(&cmd);
			else if (!str[i])
				break ;
			else
				return (i + 1);
		}
		i++;
	}
	return (i + 1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:54:35 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:20:58 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	pipex_error_make_redir(int pipe[2])
{
	if (flag == 3)
		exit(1);
	else
	{
		close (pipe[1]);
		return (pipe[0]);
	}
}

int	ft_last_child(char *cmd, int fd)
{
	int		pipe[2];
	int		pid;
	t_input	input;

	pipe_with_error_check(pipe);
	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		return (pipex_error_make_redir(pipe));
	pid = fork_with_error_check();
	if (!pid)
	{
		close(pipe[1]);
		ft_is_valid_in(fd, &input);
		close(fd);
		exit(executer(cmd, &input));
	}
	close(fd);
	close(pipe[1]);
	ft_wait_all_children(pid);
	return (0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mid_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:53:29 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:21:23 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_mid_child(char *cmd, int fd)
{
	int		pipe[2];
	int		pid;
	t_input	input;

	pipe_with_error_check(pipe);
	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		return (pipex_error_make_redir(pipe));
	pid = fork_with_error_check();
	if (!pid)
	{
		ft_is_valid_in(fd, &input);
		close(fd);
		dup2_with_error_check(pipe[1], input.out);
		close(pipe[1]);
		exit(executer(cmd, &input));
	}
	close(input.in);
	close(input.out);
	close(fd);
	close(pipe[1]);
	return (pipe[0]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:56:48 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 17:45:49 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static	void	pipex_init_pipex(t_pipstr *pipex, int *fd, char *cmd)
{
	*fd = 0;
	pipex->i = 1;
	pipex->cmd_cpy = cmd;
	pipex->n = count_pipes(cmd);
	pipex->status = 0;
}

void	pipex(char *cmd)
{
	t_pipstr	pipex;
	int			pid;
	int			fd;
	int			status;

	status = 0;
	pid = fork_with_error_check();
	if (!pid)
	{
		pipex_init_pipex(&pipex, &fd, cmd);
		pipe_with_error_check(pipex.pipes.start_pipe);
		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		fd = ft_first_child(pipex.cmd, pipex.pipes.start_pipe);
		while (pipex.i < pipex.n)
		{
			pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
			fd = ft_mid_child(pipex.cmd, fd);
			pipex.i += 1;
		}
		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		pipex.status = ft_last_child(pipex.cmd, fd);
	}
	waitpid(-1, &status, 0);
	exit (WEXITSTATUS(status));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:52:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 16:25:19 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_is_valid_in(int fd, t_input *input)
{
	char	**split;
	size_t	i;
	int		pipes[2];

	i = 0;
	split = ft_lexer(input->cmd);
	if (!split)
		return ;
	while (split[i])
		i++;
	ft_free_split_2(&split);
	if (i == 1 && input->here)
	{
		pipe_with_error_check(pipes);
		close(pipes[1]);
		dup2_with_error_check(input->in, pipes[0]);
		return ;
	}
	dup2_with_error_check(fd, input->in);
	return ;
	(void)pipes;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_all_childs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:54:05 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 18:54:20 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_wait_all_children(int pid)
{
	int	id;
	int	status;
	int	final;

	status = 0;
	waitpid(pid, &status, 0);
	final = status;
	while (1)
	{
		id = waitpid(-1, &status, 0);
		if (id < 0)
			break ;
	}
	exit(WEXITSTATUS(final));
	pid = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 16:21:28 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 20:13:01 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_get_old_history(char **env, int *fd)
{
	int		i;
	char	*str;
	char	*str2;
	char	*str3;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		if (!ft_strncmp("HOME=", env[i], 5))
			break ;
		i++;
	}
	if (!env[i])
		return ;
	str = *(env + i);
	str += 5;
	str2 = ft_strjoin(str, "/.minishell_history");
	*fd = open(str2, O_CREAT | O_RDWR | O_APPEND, 0644);
	ft_free((void *)&str2);
	if (*fd < 0)
		return ;
	str3 = get_next_line(*fd);
	while(str3)
	{
		str3[ft_strlen(str3) - 1] = '\0';
		if (str3)
			add_history(str3);
		ft_free((void *)&str3);
		str3 = get_next_line(*fd);
	}
}

int	len_2_fill(int x, int new)
{
	static int	value = 0;

	if (x)
		value = new;
	return (value);
}

void	save_and_clear(void *content)
{
	char		*str;
	static int	i = 0;

	str = (char *)content;
	if (!str)
		return ;
	if (*str && (!((ft_get_history()) == STDIN_FILENO)))
	{
		if (i >= len_2_fill(0, 0))
		{
			ft_putstr_fd(str, ft_get_history());
			ft_putstr_fd("\n", ft_get_history());
		}
	}
	i++;
	ft_free((void **)&str);
	return ;
}

char	*ft_get_history_path(char **env)
{
	size_t	i;
	char	*str;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		if (!ft_strncmp("HOME=", env[i], 5))
			break ;
		i++;
	}
	if (!env[i])
		return (NULL);
	str = *(env + i);
	str += 5;
	return (ft_strjoin(str, "/.minishell_history"));
}

void	ft_save_history_in_list(int fd, t_list **list)
{
	char	*str;

	str = get_next_line(fd);
	while (str)
	{
		str[ft_strlen(str) - 1] = '\0';
		ft_lstadd_back(list, ft_lstnew((void *)ft_strdup(str)));
		ft_free((void *)&str);
		str = get_next_line(fd);
	}
	return ;
}

int	fd_old_history_4_list(char **env, t_save_history *his)
{
	int		fd;
	char	*str2;

	str2 = ft_get_history_path(env);
	if (!str2)
		return (-1);
	fd = open(str2, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
	ft_save_history_in_list(fd, &his->old_history);
	ft_lstadd_back(&his->old_history, his->history);
	close(fd);
	fd = open(str2, O_CREAT | O_RDWR | O_TRUNC, 0644);
	ft_free((void *)&str2);
	return (fd);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_history.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 19:49:06 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/28 19:27:23 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_save_history(void)
{
	t_save_history	his;
	
	his.old_history = NULL;
	his.history = (ft_get_static())->history;
	his.fd = ft_get_history();
	if (his.fd != -1 && his.fd != STDIN_FILENO)
		close(his.fd);
	his.env = list_to_matrix((ft_get_static())->env);
	his.fd = fd_old_history_4_list(his.env, &his);
	ft_free_split_2(&(his.env));
	if (his.fd == -1)
		return ;
	his.len = ft_lstsize(his.old_history);
	his.len = his.len - 501;
	if (his.len < 0)
		his.len = 0;
	len_2_fill(1, his.len);
	ft_put_history(his.fd);
	ft_lstclear(&his.old_history, save_and_clear);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   holaaaa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:11:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 15:26:38 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

char	**ft_lexer(char *str)
{
	char			**result;
	int				len;

	if (ft_check_argument(str) == -1)
		return (NULL);
	len = ft_lexer_len_n_arguments(str);
	result = ft_calloc(sizeof(char *), (len + 1));
	if (!result)
		return (NULL);
	result[len] = NULL;
	ft_alloc_parse_result(&result, str, len);
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:05:02 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:57:48 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	count_arguments(char *str)
{
	int				i;
	int				command;
	int				n_commands;

	if (!str)
		return (0);
	i = 0;
	n_commands = 0;
	command = 0;
	while (str[i] == ' ')
		i++;
	if (str[i])
		n_commands = 1;
	while (str[i])
	{
		while (str[i] != '/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:56:33 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/12 17:03:40 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	handler(int signal)
{
	if (flag == PROCCESS)
		errno = 130;
	if (flag == 4)
		exit(130);
	if (flag != HERE)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		if (flag == SHELL)
			rl_redisplay();
	}
	else
		exit(2);
	return ;
	signal = 0;
}

void	quit_signal(int signal)
{
	if (flag == SHELL)
		rl_redisplay();
	return ;
	signal = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_static_struct.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:34:53 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:19:22 by shujiang         ###   ########.fr       */
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

void	ft_update_shlvl(char **env)
{
	int		i;
	char	*shlvl;
	int		nb;
	char	*new_shlvl;

	i = 0;
	new_shlvl = NULL;
	nb = 0;
	shlvl = NULL;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "SHLVL=", 6))
		{
			shlvl = ft_substr(env[i], 6, ft_strlen(env[i]));
			nb = ft_atoi(shlvl);
			free(shlvl);
			nb++;
			new_shlvl = ft_itoa(nb);
			env[i] = ft_strjoin("SHLVL=", new_shlvl);
			free(new_shlvl);
			break ;
		}
		i++;
	}
}

t_static	*ft_setup_struct(char **env, t_static *s)
{
	if (s->here == -1)
	{
		perror("dup: ");
		return (NULL);
	}
	if (!(*env))
	{
		s->oldpwd = ft_lstnew("OLDPWD");
		s->last_cmd = ft_lstnew("_=./minishell");
	}
	else
	{
		ft_update_shlvl(env);
		s->oldpwd = ft_lstnew(ft_get_info_from_env(env, "OLDPWD="));
		s->last_cmd = ft_lstnew(ft_get_info_from_env(env, "_="));
	}
	return (s);
}

t_static	*init_static_struct(char **env)
{
	t_static	*s;
	int			*i;
	char		pwd[4096];

	s = calloc(1, sizeof(t_static));
	if (!s)
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
	return (ft_setup_struct(env, s));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 13:08:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:13:14 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_static	*ft_get_static(void)
{
	return (ft_static(0, NULL));
}

t_static	*ft_put_static(t_static *new)
{
	return (ft_static(1, new));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:14:00 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:27:17 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_static_history(int fd_n, int i)
{
	static int	fd = -1;

	if (!i)
		fd = fd_n;
	return (fd);
}

int	ft_get_history(void)
{
	return (ft_static_history(0, 1));
}

void	ft_put_history(int n)
{
	ft_static_history(n, 0);
}

int	ft_static_proccess(int fd_n, int i)
{
	static int	fd = 0;

	if (!i)
		fd = fd_n;
	return (fd);
}

void	ft_put_proccess(int n)
{
	ft_static_proccess(n, 0);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:13:20 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/28 13:18:48 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_get_error(void)
{
	return (*(int *)(ft_get_static())->error->content);
}

void	ft_put_error(int error)
{
	*(int *)(ft_get_static())->error->content = error;
}

t_static	*ft_static(int modify, t_static *new)
{
	static t_static	*s = NULL;

	if (modify == 1)
		s = new;
	return (s);
}

int	ft_get_proccess(void)
{
	return (ft_static_proccess(0, 1));
}
