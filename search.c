/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/07 14:49:21 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	leaks()
{
	system("leaks -q minishell");
}

void	*ft_free(void **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
	return (0);
}

void	ft_free_input(char **input)
{
	int i;
	i = 0;
	while (input[i])
	{
		free(input[i]);
		i++;
	}
	free (input);
}

void	*ft_print_error(char *str, int error)
{
	write(2, "\7minishell: ", 12);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	ft_put_error(error);
	errno = error;
	return (NULL);
}

/* void	ft_shlvl_sum(void)
{
	char *var = "SHLVL";
	t_static *s;
	char *env_var;
	size_t len;
	
	s = ft_get_static();
	len = ft_strlen(var);
	while (s->env)
	{
		env_var = s->env->content;
		if(env_var && var && !ft_strncmp(env_var, var, len) 
			&& env_var[ft_strlen(var)]== '=')
		{
			int number;
			char	*str;
			extern char **environ;

			number = ft_atoi(env_var + len + 1);
			number++;
			str = ft_itoa(number);
			s->shlvl->content = str;
			//printf("%s\n", ft_lstnew(ft_strjoin("SHLVL=", str))->content);
			add_list_and_sort(&(s->env), ft_lstnew(ft_strjoin("SHLVL=", str)));
			environ[0] = ft_strdup(env_var + len + 1);
			return ;
		}
		s->env = s->env->next;
	}
	return ;
} */



int shell_mode(char **env)
{
	char	*line;
	int		fd_mini_history;
	
	t_list	*history;
	t_static *s;
	flag = 0;
	fd_mini_history = 0;
	//atexit(leaks);
	
	ft_get_old_history(env, &fd_mini_history);
	line = NULL;
	signal(SIGINT, handler);
	signal(SIGQUIT, quit_signal);
	//if (s->shlvl)
	//	shlvl++;
	ft_put_static(init_static_struct(env));
	s = ft_get_static();
	history = s->history;
	printf("%s\n", s->shlvl->content);
	ft_copy_env(env);

	creat_exp_list(s);
	add_list_and_sort(&(s->exp), ft_lstnew(ft_strjoin("declare -x ",s->shlvl->content)));
	//export_to_real_env(s);
	//ft_shlvl_sum();
	
	ft_put_error(0);
	//creat_exp_list(s);
	flag = SHELL;
	//ft_shlvl_sum();
	while (1)
	{
		if (flag != 3)
			flag = SHELL;
		//line = readline("minishell$ ");
		if (isatty(fileno(stdin)))
		{
			if (flag != 3)
				line = readline("minishell$ ");
			else
			{
				line = readline("");
				flag = 0;
			}
		}
		else
		{
			char *line2;
			line2 = get_next_line(fileno(stdin));
			line = ft_strtrim(line2, "\n");
			free(line2);
		}
		if (!line)
		{
	//		write(STDERR_FILENO, "exit\n", 5);
			ft_free((void *)&line);
			//printf("this is the error:%d\n", ft_get_error());
			return(ft_get_error());
		}
		
		add_history(line);
		ft_lstadd_back(&history, ft_lstnew((void *)ft_strdup(line)));
		if (ft_check_argument(line) == 1)
		{
			ft_procces_maker(line, env);
			ft_put_proccess(0);
		}
		else
			ft_free((void *)&line);
	}
	return (ft_get_error());
}

int	exc_mode(char *file, char **env)
{
	char *str;
	char *gnl;

	errno = 0;
	ft_put_static(init_static_struct());
	int fd = 0;
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		str = ft_strjoin("minishell: ", file);
		perror(str);
		ft_free((void **)&str);
		exit(errno);
	}
	size_t	i;

	i = 0;
	gnl = get_next_line_samu(fd);
	while (gnl)
	{
		i = 0;
		while (gnl[i])
		{
			if (gnl[i] == '\n')
			{
				gnl[i] = '\0';
				break ;
			}
			i++;
		}
		if (ft_check_argument(gnl) == 1)
			ft_procces_maker(gnl, env);
		gnl = get_next_line_samu(fd);
		ft_free((void **)&gnl);
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	if (argc == 1)
		return (shell_mode(env));
	else 
		return (exc_mode(argv[1], env));
}

//This is a main to test the fuction add_list_and_sort
/* int main()
{
    t_list *list;
    char pwd[4096];

    t_list *temp;

    getcwd(pwd, sizeof(pwd));
    list = malloc(sizeof(t_list));
    list = ft_lstnew(ft_strjoin("PWD=",pwd));
	add_list_and_sort(&list, ft_lstnew("Bpple="));
    add_list_and_sort(&list, ft_lstnew("_=./minishell"));
    add_list_and_sort(&list, ft_lstnew("SHLVL=1"));
	add_list_and_sort(&list, ft_lstnew("Aanana"));
	add_list_and_sort(&list, ft_lstnew("Bpple=9"));
	add_list_and_sort(&list, ft_lstnew("AAA=9"));
	add_list_and_sort(&list, ft_lstnew("Aanana = BANANA"));
    
    temp = list;
    while(temp)
    {
        printf("%s\n", temp->content);
        temp = temp->next;
    }
    return (0);
}  */

/* This main is for testing env
int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	
	t_static *s;
    ft_put_static(init_static_struct());
	s = ft_get_static();
	ft_copy_env(env);
	print_env_cpy();
    return (0);
} */
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 16:53:23 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/06 20:48:59 by shujiang         ###   ########.fr       */
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
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 48 && str[i] <= 57)
			i++;
		break ;
	}
	if (i != (int)ft_strlen(str))
		return (0);
	return (1);
}

void ft_free_exit(char **input, int n)
{
	ft_free_input(input);
	exit (n);
}

void ft_exit(char    **input)
{
	int i;

	i = 0; 
	printf("exit\n");
	if (input && input[1] == NULL)
		ft_free_exit(input, 0);
	if (input[2] != NULL)
	{
		printf("minishell: exit: too many arguments\n");
		ft_put_error(255);
	}
	else if (input[1] != NULL)
	{
		if(!check_digit(input[1])) 
		{
			printf("minishell: exit: %s: numeric argument required\n", input[1]);
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 18:04:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/06 19:59:47 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void    get_default_env(t_static *s)
{
    char buf[4096];
    
    s->env= ft_lstnew(ft_strjoin("PWD=",getcwd(buf, sizeof(buf))));
	add_list_and_sort(&(s->env),ft_lstnew(s->last_cmd->content));
	add_list_and_sort(&(s->env),ft_lstnew(s->oldpwd->content));
    add_list_and_sort(&(s->env),ft_lstnew(s->shlvl->content));
}
void ft_copy_env(char **env)
{
    int i;
    t_list *new;
    t_static *s;

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

void    print_env_cpy(void)
{
    t_list *temp;
    char *value;
    t_static *s;

	s = ft_get_static();   
    temp = s->env;
    value = NULL;    
    while(temp)
    {
        value = ft_strchr(temp->content, '=');
        //if (value && *(value + 1))
        if (ft_strcmp("OLDPWD", temp->content))
            printf("%s\n", temp->content);
        temp = temp->next;
    }
}

/* void    print_env_cpy(void)
{
    int i;
    t_list *temp;
    t_static *s;

	s = ft_get_static();
    i = 0;
    temp = s->env;
    while(temp)
    {
        if (temp->content && *(ft_strchr(temp->content, '=') + 1))
            printf("%s\n", temp->content);
        temp = temp->next;
    }
} */

void    ft_env(char **input)
{
   
    if (input[1] != NULL)
	{
		printf("env: %s: No such file or directory\n", input[1]);
        return ;
	}
    print_env_cpy();
}


/* void update_shlvl()
{
    t_static *s;
    
    s = ft_get_static();        
} *//* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 18:49:20 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/05 18:13:27 by shujiang         ###   ########.fr       */
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
			printf("minishell: export: %s: not a valid identifier\n", str);
			//free;
			return (0);
		}
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

void add_new_var_exp(char *str)
{
	t_list *new;
	//t_list *temp;
	char *new1;
	char *new2;
	char *new3;
	t_static *s;

	s = ft_get_static();
	new1 = NULL;
	new2 = NULL;
	new3 = NULL;
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
		{
			new1= ft_substr(str, 0, var_len(str) + 1);
			new2 = ft_strjoin("declare -x ", new1);
			free (new1);
			new1 = ft_strjoin(new2, "\"");
			free (new2);
			new2 = ft_substr(str, var_len(str) + 1, ft_strlen(str) - var_len(str) + 1);
			new3 = ft_strjoin(new2, "\"");
			free (new2);
			new2 = ft_strjoin(new1 , new3);
		}
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
		if (ft_strncmp(old + 11, str, var_len(str)) == 0 
			&& ((old + 11)[var_len(str)] == '\0' || (old + 11)[var_len(str)] == '=' ))
			break ;
		temp = temp->next;
	}
	if (temp)
	{
		new1= ft_substr(old, 0, 11 + var_len(str) + 1);
		if (!ft_strchr(old, '='))
			new2 = ft_strjoin(new1, "=\"");
		else 
			new2 = ft_strjoin(new1, "\"");
		free (new1);
		new1 = ft_strjoin(str + var_len(str) + 1, "\"");
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
	temp = s->env;
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
	char *old;
	t_static *s;
	
	s = ft_get_static();
	i = 1;
	var = NULL;
	old = NULL;
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
					{
						add_new_var_env(var);
						printf("add new var to env\n");
					}
					else
					{
						modify_env(var);
						printf("modify env\n");
					}	
				}
			}
		}
		i++;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:30:13 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/06 19:49:04 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int ft_var_len(char *var)
{
    int i;

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

t_list *ft_locate_node(t_list *list, char *var_name)
{
    t_list *temp;
    int len;
    char *str;


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
        if (!ft_strncmp(str, var_name, len) && (str[len] == '=' || str[len] == '\0'))
            return (temp);
        temp = temp ->next;
    }
    return (NULL);
}

void    ft_remove_node(t_list *list, int index)
{
    t_list *before;
    int i;
    t_list *after;
    t_list *temp;

    i = 0;
    temp = list;
    while (i < index - 1)
    {
       temp = temp ->next;
       i++; 
    }
    before = temp;
    temp = temp->next;
    after = temp->next;
    before->next = after;
    free(temp);
}

void    ft_node_substitute(t_list **old, t_list **new)
{
    if(*old && *new)
        (*old)->content = ft_strdup((*new)->content); 
}

void    ft_front_insert(t_list** front_node, t_list **new)
{
    t_list *back_node;
    
    back_node = (*front_node)->next;
    (*front_node)->next = *new;
    (*new)->next = back_node;
}

int   check_put_new_node_first(t_list **list, t_list *new)
{
	int  len;
    char *str;
    char *strnew;

    str = (*list)->content;
	strnew = new->content;
    len = ft_var_len(strnew);
    if (ft_strncmp(str, strnew, len) == 0 && str[len]== '=' && strnew[len] == '=')
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

//len = ft_var_len(insert) + 1; check until '='
//add or substitute and sort
void    add_list_and_sort(t_list **list, t_list *new)
{
    int  len;
    t_list *temp;
    char *str;
    char *strnew;
    
    len = 0;
    strnew = NULL;
    temp = *list;
    str = (*list)->content;
    //printf("str %s\n",str);
    if (new)
	{
        strnew = new->content;
        //printf("strnew %s\n",strnew);
        len = ft_var_len(strnew);
        //printf("len %d\n",len);
    }    
    if (check_put_new_node_first(list, new) == 1)
		return ;
    while (temp)
    {
        /* printf("%d\n",len);
         printf("%c\n",str[len]);
        printf("%c\n",strnew[len]); */  
        if (ft_strncmp(str, strnew, len) == 0 && str[len]== '=' && strnew[len] == '=')
        {
            /* printf("%s\n",str);
            printf("%s\n",strnew); */
            ft_node_substitute(&(temp->next), &new);
            break ;
        }
        if (temp->next && ft_strcmp(temp->next->content, new->content) > 0)
        {
            ft_front_insert(&temp, &new);
            break ;
        }
        temp = temp->next;
        if(!temp)   
            ft_lstadd_back(list, new);
    } 
}   

int ft_count_node(t_list *list)
{
    int i;
    t_list *temp;
    
    i = 0;
    temp = list;
    while (temp)
    {
        i++;
        temp = temp->next;
    }
    return (i);
}

char **list_to_matrix(t_list *list)
{
	char **matrix;
    int n;
    int i;
    t_list *temp;

    i = 0;
    n = ft_count_node(list);
    temp = list;
    matrix = (char **)malloc(sizeof(char *) * (n + 1));
    if (!matrix)
        return (NULL); 
    while (i < n)
    {
        matrix[i] = ft_strdup((char *)temp->content);
        temp=temp->next;
        i++;
    }
    matrix[i] = NULL;
    i = 0;
    return (matrix);
}

/* int main()
{
    t_list *list;
    char pwd[4096];

    t_list *temp;

    getcwd(pwd, sizeof(pwd));
    list = malloc(sizeof(t_list));
    list = ft_lstnew(ft_strjoin("PWD=",pwd));
	add_list_and_sort(&list, ft_lstnew("Bpple="));
    add_list_and_sort(&list, ft_lstnew("_=./minishell"));
    add_list_and_sort(&list, ft_lstnew("SHLVL=1"));
	add_list_and_sort(&list, ft_lstnew("Aanana"));
	add_list_and_sort(&list, ft_lstnew("Bpple=9"));
	
    
    temp = list;
    while(temp)
    {
        printf("%s\n", temp->content);
        temp = temp->next;
    }
    return (0);
} *//* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 18:05:01 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/05 19:48:47 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	get_var_index_exp(char *var_name)
{
	int len;
	t_list *temp;
	int index;
	char *ref;
	t_static *s;

	s = ft_get_static();
	index = 0;
	temp = s->exp;
	len = ft_strlen(var_name);
	ref = temp->content + 11;
	while(temp)
	{
		if(ft_strncmp(ref, var_name, len - 1) == 0 && ref[len] == '=')
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
	int len;
	t_list *temp;
	int index;
	char *ref;
	t_static *s;

	s = ft_get_static();
	index = 0;
	temp = s->env;
	len = ft_strlen(var_name);
	ref = temp->content;
	while(temp)
	{
		if(ft_strncmp(ref, var_name, len - 1) == 0 && ref[len] == '=')
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

/* void	unset_var(int index)
{
	t_list **ptr;
	t_list *node_to_remove;
	t_static *s;

	s = ft_get_static();
	node_to_remove = NULL;
	ptr = &(s->env);
	while(index > 0 && *ptr != NULL)
	{   
		ptr = &((*ptr)->next);
		index--;
	}
	if (*ptr != NULL)
    {
        node_to_remove = *ptr;
        *ptr = (*ptr)->next;
        free(node_to_remove);
    }
} */

void	unset_var(char *var_name, int index, t_list *list)
{
	t_list *node_to_remove;
	t_static *s;

	s = ft_get_static();
	node_to_remove = ft_locate_node(list, var_name);
	ft_remove_node(list, index);
}

void    ft_unset(char **input)
{
	int i;
	int index1;
	int index2;
	t_static *s;

	s = ft_get_static();
	i = 1;
	if(input[i] == NULL)
		return ;
	while (input[i])
	{		
		index1 = get_var_index_env(input[i]);
		index2 = get_var_index_exp(input[i]);
		printf("2: %d\n", index2);
		if (index1 >= 0)
			unset_var(input[i], index1, s->env);
		if (index2 >= 0)
			unset_var(input[i], index2, s->exp);
		i++;	
	}	
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/07 14:11:12 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int		ft_exc_execution(char *cmd, char **env)
{
	char **input;

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

int	ft_error_exc_unexpected_token(int minor, int major, char first)
{	
	if ((minor > 2) || (major == 2 && !minor))
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

int	ft_check_parse_normal_char(t_exc_lex *lex)
{
	if (lex->major > 2 || lex->minor > 2 || lex->redirs > 2)
	{
		ft_error_exc_unexpected_token(lex->minor, lex->major, lex->first);
		return (-1);
	}
	lex->major = 0;
	lex->minor = 0;
	lex->redirs = 0;
	return (0);
}

int	ft_check_parse_redirs(t_exc_lex *lex)
{
	if (lex->input[lex->i] == '<' && !lex->major)
	{
		if (!lex->redirs)
			lex->first = '<';
		lex->minor += 1;
		lex->redirs += 1;
	}
	else if (lex->input[lex->i] == '<' && lex->major)
		return (ft_error_exc_unexpected_token(lex->minor, lex->major, lex->first));
	if (lex->input[lex->i] == '>' && !lex->minor)
	{
		if (!lex->redirs)
			lex->first = '>';
		lex->major += 1;
		lex->redirs += 1;
	}
	else if (lex->input[lex->i] == '>' && lex->minor)
		return (ft_error_exc_unexpected_token(lex->minor, lex->major, lex->first));
	if (lex->major > 2 || lex->minor > 2 || lex->redirs > 2)
		return (ft_error_exc_unexpected_token(lex->minor, lex->major, lex->first));
	return (0);
}

int	ft_check_dup_redir(char *input)
{
	t_exc_lex	lex;

	lex.input = input;
	ft_init_exc_lex(&lex);
	while (input[lex.i])
	{
		lex.j = ft_check_char(&lex.cmd, input[lex.i]);
		if (!lex.j || lex.j == -1)
		{
			ft_init_cmd(&lex.cmd);
		}
		else
		{
			if(ft_check_parse_normal_char(&lex) == -1)
				return (-1);
		}
		if (lex.j == -1 && (input[lex.i] == '<' || input[lex.i] == '>'))
		{
			if(ft_check_parse_redirs(&lex) == -1)
				return (-1);
		}
		lex.i += 1;
	}
	return (0);
}

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

size_t	ft_strlen2(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while(str[i])
		i++;
	return (i);
}

int	ft_exc_here_doc(t_argument *content, t_exc_lex *lex)
{
	int		pipes[2];
	int		pid;
	int		status;

	if(pipe(pipes))
		exit(errno);
	content->type = ft_strdup("hre");
	pid = fork();
	if (!pid)
	{
		char	*str;

		flag = HERE;
		write((int)((ft_get_static())->here), "> ", 2);
		str = get_next_line((int)((ft_get_static())->here));
		//printf("str:%s", str);
		if (!str)
			exit (0);
		while (1)
		{
			if (!ft_strncmp(content->str, str, ft_strlen2(content->str)))
				exit(1);
			//printf("str:%s", str);
			write((int)((ft_get_static())->here), "> ", 2);
			ft_putstr_fd(str, pipes[1]);
			free(str);
			str = get_next_line((int)((ft_get_static())->here));
			if (!str)
				exit (0);
		}
		free(str);
		exit(0);
	}
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

int	ft_exc_change_input(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("inp");
	fd = open(content->str, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell: ");
		return (-1);
	}
	lex->in = fd;
	return (fd);
}

int	ft_exc_change_output_trc(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("trc");
	fd = open(content->str, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: ");
		return (-1);
	}
	lex->in = fd;
	lex->out = fd;
	return (fd);
}

int	ft_exc_change_output_apd(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("apd");
	fd = open(content->str, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("minishell: ");
		return (-1);
	}
	lex->out = fd;
	return (fd);
}

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

void	*ft_exc_free_content(void *cnt_ptr)
{
	t_argument	*content;

	content = (t_argument *)cnt_ptr;
	if (ft_strncmp(content->type, "cmd", 3))
		close(content->fd);
	ft_free((void **)&content->str);
	ft_free((void **)&content->type);
	return (NULL);
}

void	*ft_exc_clear_content(t_list **result)
{
	ft_lstclear(result, \
	(void (*)(void *))&ft_exc_free_content);
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
	{
		return (ft_exc_free_content((void *)content));
	}
	result = ft_lstnew((void *)content);
	if (!result)
		return (ft_exc_free_content((void *)content));
	return (result);
}

char *ft_exc_make_word(char *input)
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

void	*ft_error_make_list(t_list **result, t_exc_lex *lex, int error)
{
	ft_init_exc_lex(lex);
	ft_exc_clear_content(result);
	if (error)
		ft_error_exc_unexpected_token(0, 0, 0);
	return (NULL);
}

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
	return(result);
}

void *ft_not_closed_pipe(char **env)
{
	char	*str;
	int		i;

	i = 0;
	write((int)((ft_get_static())->here), ">", 1);
	
	str = get_next_line((int)((ft_get_static())->here));
	if (!str)
	{
		ft_print_error("syntax error: unexpected end of file", 258);
		return (NULL);
	}
	while (str[i] != '\n')
		i++;
	if (!i)
	{
		free(str);
		return (ft_not_closed_pipe(env));
	}
	str[i] = '\0';
	if (ft_check_argument(str) == 1)
		ft_procces_maker(str, env);
	return (NULL);
}

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

char	*ft_exc_make_cmd(t_list **input)
{
	char	*result;
	char	*tmp1;
	t_list	*ptr;
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

int ft_exc_make_redir(char *cmd, t_input *line)
{
	int		std[2];
	t_list	*input;

	std[0] = STDIN_FILENO;
	std[1] = STDOUT_FILENO;
	input = ft_exc_lex_input(cmd, std);
	if (!input)
		return (errno);
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

int	executer(char *cmd, t_input *input)
{
	int		cloud[2];
	int		value;
	char	**env;
	t_static *s;

	s = ft_get_static();
	env = NULL;
	env = list_to_matrix(s->env);

	cloud[0] = dup(0);
	cloud[1] = dup(1);
	//value = ft_exc_make_redir(cmd, env);
	value = ft_executer_exec(input, env);
	dup2_with_error_check(cloud[1], 1);
	close(cloud[1]);
	dup2_with_error_check(cloud[0], 0);
	close(cloud[0]);
	ft_free((void **)&cmd);
	return (value);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proccess.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:16:47 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/07 14:11:16 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

size_t	get_next_index_pipex(char *str)
{
	int	j;
	int	i;
	int	n;
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

size_t	count_pipes(char *str)
{
	int	j;
	int	i;
	int	n;
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

int	ft_first_child(char *cmd, int pipe[2])
{
	int	pid;
	int	status;
	t_input	input;

	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		exit(pid);
	pid = fork_with_error_check();
	if(!pid)
	{
		close(pipe[0]);
		dup2_with_error_check(pipe[1], input.out);
		close(pipe[1]);
		exit(executer(cmd, &input));
	}
	close(pipe[1]);
	//waitpid(-1, &status, 0);
	return (pipe[0]);
	status = 0;
}

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
	//exit(0);
	dup2_with_error_check(fd, input->in);
	return ;
	(void)pipes;
}

int	ft_mid_child(char *cmd, int fd)
{
	int	pipe[2];
	int	pid;
	int	status;
	t_input	input;

	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		exit(pid);
	pipe_with_error_check(pipe);
	pid = fork_with_error_check();
	if(!pid)
	{
		ft_is_valid_in(fd, &input);
		//dup2_with_error_check(fd, input.in);
		close(fd);
		dup2_with_error_check(pipe[1], input.out);
		close(pipe[1]);
		exit(executer(cmd, &input));
	}
	close(fd);
	close(pipe[1]);
	//waitpid(-1, &status, 0);
	return (pipe[0]);
	status = 0;
}

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

int	ft_last_child(char *cmd, int fd)
{
	int	pipe[2];
	int	pid;
	t_input	input;

	pid = ft_exc_make_redir(cmd, &input);
	if (pid)
		exit(pid);
	pipe_with_error_check(pipe);
	pid = fork_with_error_check();
	if(!pid)
	{
		close(pipe[1]);
		ft_is_valid_in(fd, &input);
		//dup2_with_error_check(fd, input.in);
		close(fd);
		exit(executer(cmd, &input));
	}
	close(fd);
	close(pipe[1]);
	ft_wait_all_children(pid);
	//waitpid(-1, &status, 0);
	return (0);
}

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

void	pipex(char *cmd)
{
	t_pipstr	pipex;
	int			pid;
	int			status;
	int			fd;

	status = 0;
	fd = 0;
	pid = fork_with_error_check();
	if (!pid)
	{
		pipex.i = 1;
		pipex.cmd_cpy = cmd;
		pipex.n = count_pipes(cmd);
		pipex.status = 0;
		pipe_with_error_check(pipex.pipes.start_pipe);

		//first child execution

		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		fd = ft_first_child(pipex.cmd, pipex.pipes.start_pipe);
	
		//mid childs execution

		while (pipex.i < pipex.n)
		{
			pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
			fd = ft_mid_child(pipex.cmd, fd);
			pipex.i += 1;
		}

		//last child execution

		pipex.cmd = ft_get_cmd_pipex(&pipex.cmd_cpy);
		pipex.status = ft_last_child(pipex.cmd, fd);
		close(0);
		close(pipex.pipes.start_pipe[1]);
		close(pipex.pipes.start_pipe[0]);
		close(fd);
		//printf("status before before pipes:%d, %d\n", WEXITSTATUS(pipex.status), pipex.status);
		exit (pipex.status);
	}
	waitpid(-1, &status, 0);
	//printf("status before pipes:%d, %d\n", WEXITSTATUS(status), status);
	exit (WEXITSTATUS(status));
}

void	ft_procces_maker(char *cmd, char **env)
{
	char **input;
	int		pid;
	int		status;
	t_input	line;

	input = ft_lexer(cmd);
	if (input)
	{
		ft_free_split_2(&input);
		if (count_pipes(cmd) > 0)
		{
			flag = PROCCESS;
			pid = fork_with_error_check();
			ft_put_proccess(1);
			if (!pid)
				pipex(cmd);
			waitpid(-1, &status, 0);
			ft_put_error(WEXITSTATUS(status));
		}
		else
		{
			pid = ft_exc_make_redir(cmd, &line);
			if (pid)
			{
				ft_put_error(pid);
				return ;
			}
			ft_put_error(executer(cmd, &line));
		}
	}
	else
		ft_free_split_2(&input);
	env = NULL;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   holaaaa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:11:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/05 20:31:47 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

char **ft_lexer(char *str)
{
	char			**result;
	int				len;
	int				i;

	if (ft_check_argument(str) == -1)
		return (NULL);
	len = ft_lexer_len_n_arguments(str);
	// y si len da 0 que se hace???
	result = ft_calloc(sizeof(char *), (len + 1));
	if (!result)
		return (NULL);
	result[len] = NULL;
	ft_alloc_parse_result(&result, str, len);
	return (result);
	str = NULL;
	i = 0;
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
/*   Updated: 2023/09/05 15:36:25 by samusanc         ###   ########.fr       */
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
		if (!ft_strncmp((char *)strc.tmp->content, strc.str2, ft_strlen(strc.str2)))
			break ;
		strc.tmp = strc.tmp->next;
	}
	if(!ft_dollar_fill_util(&strc))
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
/*   Updated: 2023/09/05 15:36:03 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_dollar_len(char *str, t_cmd cmd)
{
	int	j;
	int	i;
	char	*str2;
	t_list	*tmp;

	j = 4;
	i = 0;
	if (!str)
		return (0);
	if (!str[0])
		return (1);
	j = ft_check_char(&cmd, str[i++]);
	while (j == 3)
		j = ft_check_char(&cmd, str[i++]);
	if (i == 1)
		return (1);
	str2 = malloc(sizeof(char) * (i + 1));
	if (!str2)
		return (0);
	ft_strlcpy(str2, str, i);
	str2[i] = '\0';
	tmp = (ft_get_static())->env;
	while (tmp)
	{
		if (!ft_strncmp((char *)tmp->content, str2, ft_strlen(str2)))
			break ;
		tmp = tmp->next;
	}
	if (!tmp)
	{
		if (str2[0] == '?')
		{
			ft_free((void **)&str2);
			str2 = ft_itoa(ft_get_error());
			i = ft_strlen(str2);
			ft_free((void **)&str2);
			return (i);
		}
		ft_free((void **)&str2);
		return (1);
	}
	ft_free((void **)&str2);
	return (ft_strlen(((char *)tmp->content + i)) + 1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:14:38 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 15:15:14 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ft_free_split_2(char ***split)
{
	int i;

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
/*   Updated: 2023/09/04 15:10:59 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//			ft_get_next_command
//		return values:
//		-4 if the command is empty
//		-3 if is the last or only command
//		-2 if is one pipe open
//		-1 if the quotes is no closed
//		(other value) index of the next command in the array

int ft_get_next_command(char *str)
{
	int				i;
	int				command;
	int				n_commands;
	int				end;

	t_command		status;
	status.dollar = funtional;
	status.simple_q = q_close;
	status.double_q = q_close;
	status.dollar = funtional;
	i = 0;
	n_commands = 0;
	command = 0;
	end = 0;
	if (!str)
		return (-1);
	while (str[i] == ' ')
		i++;
	if (str[i] == '|')
	{
		ft_print_error("syntax error near unexpected token `|'", 258);
		return (-1);
	}
	if (str[i])
		status.status = q_open;
	else
		return (-4);
	while (str[i] && !end)
	{
		while (status.status == q_open && str[i])
		{
			if(ft_lexer_check_status(&status, str, &i))
			{
				end = 1;
				break ;
			}
		}
		n_commands += 1;
		while (str[i] == ' ' && str[i] && !end)
			i++;
		if ((str[i] == '|' || str[i] == '<' || str[i] == '>'))
		{
			if(!str[i + 1])
				return (-2);
			else
				return (i + 1);
		}
		if (str[i] && !end)
		{
			if ((str[i] == '|' || str[i] == '<' || str[i] == '>'))
				break ;
			status.status = q_open;
		}
	}
	if (!str[i] && status.simple_q == q_close && status.double_q == q_close)
		status.status = q_close;
	if (status.status == q_open)
	{
		ft_print_error("syntax error unclosed quotes", 69);
		return (-1);
	}
	return (-3);
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
/*   Updated: 2023/09/04 17:35:04 by samusanc         ###   ########.fr       */
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
/*   Updated: 2023/09/04 17:35:45 by samusanc         ###   ########.fr       */
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
			//printf("new_dollar\n");
			return (4);
		}
		else
		{
			cmd->dollar_status = q_close;
			cmd->spaces = 3;
			//printf("dollar_ugly\n");
			return (3);
		}
	}
	else
	{
		cmd->dollar_status = q_close;
		cmd->spaces = 0;
		//printf("not_valid_dollar\n");
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
/*   Updated: 2023/09/04 15:17:12 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
//	1 si no es imprimible, 2 si es imprimible, 0 si es espacio y -1 es que ha acabado

int	ft_lex_quotes(t_cmd *cmd, char c)
{
	if (cmd->status == q_close)
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
	else
	{
		if (cmd->quotes == no_q)
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
		else
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
/*   Updated: 2023/09/04 15:09:43 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lexer_check_status(t_command *cmd, char *str, int *i)
{
	char	c;

	c = str[*i];
	if (cmd->status == q_open)
	{
		if (cmd->simple_q == q_close && cmd->double_q == q_close \
		&& (str[*i] == '|' || str[*i] == '<' || str[*i] == '>'))
		{
			cmd->status = q_close;
			return (1);
		}
		else if (str[*i] == '\'' ||	str[*i] == '\"')
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
			else if (str[*i] == '\'' && cmd->simple_q == q_close && cmd->simple_q == q_close)
			{
				cmd->simple_q = q_open;
				cmd->status = q_open;
			}
			else if (str[*i] == '\"' && cmd->simple_q == q_close && cmd->simple_q == q_close)
			{
				cmd->double_q = q_open;
				cmd->status = q_open;
			}
		}
		else if ((str[*i] == ' ' || !str[*i]) && cmd->simple_q == q_close && cmd->double_q == q_close)
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
/*   Updated: 2023/09/04 17:57:25 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_lexer_len_argument(char *str)
{
	t_cmd			cmd;
	int				len;
	int				i;
	int				j;

	len = 0;
	i = 0;
	j = 0;
	ft_init_cmd(&cmd);
	while (!j && str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == 2)
			len += 1;
		if (j == 4)
			len += ft_dollar_len(str + i + 1, cmd);
		i++;
	}
	if (str[i] && j >= 0)
	{
		while (str[i] && j > 0)
		{
			j = ft_check_char(&cmd, str[i]);
			if (j == 2)
				len += 1;
			if (j == 4)
				len += ft_dollar_len(str + i + 1, cmd);
			i++;
		}
	}
	return (len);
}

int	ft_lexer_len_n_arguments(char *str)
{
	t_cmd			cmd;
	int				len;
	int				i;
	int				j;
	int				x;

	len = 0;
	i = 0;
	j = 0;
	x = 0;
	ft_init_cmd(&cmd);
	while (str[i] && !j && j != -1)
	{
		j = ft_check_char(&cmd, str[i++]);
		if (j > 0)
		{
			len++;
			x = 1;
		}
	}
	while (str[i] && j >= 0)
	{
		while (str[i] && j > 0 && j != -1)
		{
			j = ft_check_char(&cmd, str[i++]);
			if (j > 0 && x != 1)
			{
				len++;
				x = 1;
			}
		}
		x = 0;
		ft_init_cmd(&cmd);
		while (str[i] && !j && j != -1)
		{
			j = ft_check_char(&cmd, str[i++]);
			if (j > 0 && x != 1)
			{
				len++;
				x = 1;
			}
		}
	}
	return (len);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:54:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 17:56:03 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>


void	ft_lexer_fill_str(char *str, char **str2)
{
	t_cmd			cmd;
	int				len;
	int				i;
	int				j;
	int				x;

	len = 0;
	i = 0;
	j = 0;
	x = 0;
	ft_init_cmd(&cmd);
	while (!j && str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		//printf("[%d] = '%c', j = %d\n", i, str[i], j);
		if (j == 2)
			str2[0][x++] = str[i];
		if (j == 4)
			ft_dollar_fill(str + i + 1, cmd, &x, str2[0] + x);
		i++;
	}
	if (str[i] && j >= 0)
	{
		while (str[i] && j > 0)
		{
			j = ft_check_char(&cmd, str[i]);
			//printf("[%d] = '%c', j = %d\n", i, str[i], j);
			if (j == 2)
				str2[0][x++] = str[i];
			if (j == 4)
				ft_dollar_fill(str + i + 1, cmd, &x, str2[0] + x);
			i++;
		}
	}
}

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
/*   Updated: 2023/09/04 17:54:43 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//	this funtions count how many numbers need per argument
void	ft_alloc_parse_result(char ***result_ptr, char *str, int len)
{
	char	**result;
	int		i;
	int		arg_len;
	char	*str2;
	int		x;

	i = 0;
	x = 0;
	result = *result_ptr;
	arg_len = 0;
	while (len)
	{
		arg_len = ft_lexer_len_argument(str);
		str2 = ft_calloc(sizeof(char) , arg_len + 1);
		if (!str2)
			return ;
		ft_lexer_fill_str(str, &str2);
		str2[arg_len] = '\0';
		if (*str2)
			result[x++] = str2;
		else
		{
			if (*str != '$')
				result[x++] = str2;
			else
				ft_free((void **)&str2);
		}
		i = ft_lexer_get_next_argument(str);
		str += i;
		len--;
	}
	return ;
	str = NULL;
	len = 0;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 11:09:21 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/04 19:43:48 by shujiang         ###   ########.fr       */
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

//get cmd, remove flags
char	*get_cmd(char *argv)
{
	/*
	char	**temp;
	char	*cmd;

	temp = ft_split(argv, ' ');
	if (*temp == NULL)
		print_error(" : command not found");
	cmd = ft_strdup(temp[0]);
	ft_free_split((void **)temp);
	*/
	char *cmd;
	cmd = ft_strdup(argv);
	return (cmd);
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
		temp = ft_strjoin(path_list[i], "/");
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
		i++;
	}
	//printf("ptr:%p\n", path_list);
	if (path_list)
		ft_free_split_2((char ***)&path_list);
	exit_cmd_not_found(path, temp, cmd);
	return (0);
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
/*   Updated: 2023/09/01 20:27:06 by samusanc         ###   ########.fr       */
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
		exit(-1);
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
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:05:51 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/31 14:35:48 by samusanc         ###   ########.fr       */
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
	static struct s_data_base	file[OPEN_MAX];
	int							read_number;
	char						*result;

	read_number = 1;
	if (read(fd, 0, 0) < 0 || fd < 0 \
	|| BUFFER_SIZE < 1 || fd > OPEN_MAX)
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
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 15:56:33 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/06 19:15:15 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	handler(int signal)
{
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
/*   Updated: 2023/09/06 20:45:31 by shujiang         ###   ########.fr       */
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
    int shlvl;
    
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
         //printf("shlvl %d\n", shlvl); 
      //printf("here %s\n", s->shlvl->content);  
    if (!(*env))
    {
        write(1,"hello\n",6);
        s->oldpwd = ft_lstnew("OLDPWD");
    	s->last_cmd = ft_lstnew("_=./minishell");
       s->shlvl = ft_lstnew("SHLVL=1");
    }
    else
    {
        s->oldpwd = ft_lstnew(ft_get_info_from_env(env, "OLDPWD="));
        s->last_cmd = ft_lstnew(ft_get_info_from_env(env, "_="));
        shlvl = ft_atoi(ft_get_info_from_env(env, "SHLVL=") + 6) + 1;
        //printf("shlvl %d\n", shlvl); 
        s->shlvl = ft_lstnew(ft_strjoin("SHLVL=",ft_itoa(shlvl)));
         
    }
    return (s);
}
   
   
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 13:08:35 by shujiang          #+#    #+#             */
/*   Updated: 2023/09/05 15:55:15 by samusanc         ###   ########.fr       */
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

int	ft_static_history(int fd_n, int i)
{
	static int	fd = 0;

	if (!i)
		fd = fd_n;
	return (fd);
}

int	ft_get_history(void)
{
	return(ft_static_history(0, 1));
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

int	ft_get_proccess(void)
{
	return(ft_static_history(0, 1));
}

void	ft_put_proccess(int n)
{
	ft_static_history(n, 0);
}

/* t_static *init_struct(char **env)
{
	t_static *s;
	int			*i;
	char buf[4096];
	const char *pwd;
	char *shlvl;

	
	s = NULL;
    s = calloc(1, sizeof(*s));
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
	s->history = NULL;
	pwd = getcwd(buf, sizeof(buf));
	s->pwd = ft_strdup(pwd);
	s->shlvl = 1;
	shlvl = ft_strjoin("SHLVL=", ft_itoa(s->shlvl));
	
	if (!env)
	{
		s->env = ft_lstnew(s->pwd);
		ft_lstadd_back(&(s->env), ft_lstnew(shlvl));
		ft_lstadd_back(&(s->env), ft_lstnew("_=./minishell"));
		creat_exp_list(s);
		return (s);
	}
    ft_copy_env(env);
    creat_exp_list(s);
	return (s);
} */

/*
t_static *init_struct(char **env)
{
	t_static *s;
	int			*i;
	char buf[4096];

	
	s = NULL;
    s = calloc(1, sizeof(*s));
    if(!s)
	{
        perror("calloc: ");
		return (NULL);
	}
	const char *pwd = getcwd(buf, sizeof(buf));
	s->pwd->content = ft_strdup(pwd);
    ft_copy_env(env);
    creat_exp_list(s);
	i = malloc(sizeof(int));
	if (!i)
	{
        perror("malloc: ");
		return (NULL);
	}
	*i = 0;
	s->error = ft_lstnew((void *)i);
	s->history = NULL;
	s->here = dup(STDIN_FILENO);
	if (s->here == -1)
	{
		perror("dup: ");
		return (NULL);
	}
	return (s);
}
*/

int	ft_get_error(void)
{
	return (*(int *)(ft_get_static())->error->content);
}

void	ft_put_error(int error)
{
	*(int *)(ft_get_static())->error->content = error;
}

t_static *ft_static(int modify, t_static *new)
{
	static t_static *s = NULL;
	if (modify == 1)
		s = new;
	return (s);
}

t_static	*ft_get_static(void)
{
	return (ft_static(0, NULL));
}

t_static	*ft_put_static(t_static *new)
{
	return (ft_static(1, new));
}
