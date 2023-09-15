/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/15 18:30:10 by samusanc         ###   ########.fr       */
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

int	ft_check_parse_normal_char(t_exc_lex *lex, int *space)
{
	if (lex->major > 2 || lex->minor > 2 || lex->redirs > 2)
	{
		ft_error_exc_unexpected_token(lex->minor, lex->major, lex->first);
		return (-1);
	}
	lex->major = 0;
	lex->minor = 0;
	lex->redirs = 0;
	*space = 0;
	return (0);
}

int	ft_check_parse_redirs(t_exc_lex *lex, int *space)
{
	if (*space)
		return (ft_error_exc_unexpected_token(lex->minor, lex->major, 'A'));
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
	int			space;
	int			word;

	space = 0;
	word = 0;
	lex.input = input;
	ft_init_exc_lex(&lex);
	while (input[lex.i])
	{
		lex.j = ft_check_char(&lex.cmd, input[lex.i]);
		if (!lex.j || lex.j == -1)
		{
			if (!lex.j)
			{
				if (lex.major || lex.minor || lex.redirs)
					space = 1;
			}
			ft_init_cmd(&lex.cmd);
		}
		else
		{
			if(ft_check_parse_normal_char(&lex, &space) == -1)
				return (-1);
		}
		if (lex.j == -1 && (input[lex.i] == '<' || input[lex.i] == '>'))
		{
			if(ft_check_parse_redirs(&lex, &space) == -1)
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
		//write((int)((ft_get_static())->here), "> ", 2);
		content->str = cut_input(content->str, NULL);
		//str = get_next_line((int)((ft_get_static())->here));
		//printf("str:%svs%s\n", str, content->str);
		str = readline("> ");
		if (!str)
			exit (0);
		while (1)
		{
			if (!ft_strncmp(content->str, str, ft_strlen2(content->str) + 1))
				exit(1);
			//aprintf("str:%svs%s\n", str, content->str);
			//write((int)((ft_get_static())->here), "> ", 2);
			ft_putstr_fd(str, pipes[1]);
			write(pipes[1], "\n", 1);
			free(str);
			//str = get_next_line((int)((ft_get_static())->here));
			str = readline("> ");
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

void	get_redir(t_argument *content)
{
	char *str;
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
	//{
	//	printf("status of stdin after be opend:%c, fd:%d\n", (int)read(content->fd, NULL, 0), content->fd);
//	}
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
