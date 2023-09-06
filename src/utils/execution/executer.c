/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/06 18:42:49 by shujiang         ###   ########.fr       */
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
	char	*str;
	int		pipes[2];

	if(pipe(pipes))
		return (-1);
	content->type = ft_strdup("hre");
	write((int)((ft_get_static())->here), ">", 1);
	str = get_next_line((int)((ft_get_static())->here));
	if (!str)
		return (-1);
	while (ft_strncmp(content->str, str, ft_strlen2(content->str)) && str)
	{
		write((int)((ft_get_static())->here), ">", 1);
		ft_putstr_fd(str, pipes[1]);
		free(str);
		str = get_next_line((int)((ft_get_static())->here));
		if (!str)
			return (-1);
	}
	free(str);
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
		return (-1);
	lex->out = fd;
	return (fd);
}

int	ft_exc_change_output_apd(t_argument *content, t_exc_lex *lex)
{
	int	fd;

	content->type = ft_strdup("apd");
	fd = open(content->str, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
		return (-1);
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
		return (ft_exc_free_content((void *)content));
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

t_list	*ft_exc_lex_input(char *input, int std[2], char **env)
{
	t_exc_lex	lex;
	t_list		*result;

	lex.input = input;
	ft_init_exc_lex(&lex);
	if (!input)
		return (ft_not_closed_pipe(env));
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

char	*ft_exc_make_cmd(char *cmd, t_list **input)
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
	cmd = NULL;
}

int ft_exc_make_redir(char *cmd, char **env)
{
	int		result;
	int		std[2];
	t_list	*input;
	char	*new_cmd;

	std[0] = STDIN_FILENO;
	std[1] = STDOUT_FILENO;
	input = ft_exc_lex_input(cmd, std, env);
	if (!input)
		return (errno);
	dup2_with_error_check(std[0], STDIN_FILENO);
	dup2_with_error_check(std[1], STDOUT_FILENO);
	new_cmd = ft_exc_make_cmd(cmd, &input);
	result = ft_exc_execution(new_cmd, env);
	close(std[0]);
	close(std[1]);
	return (result);
}

int	executer(char *cmd)
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
	
	value = ft_exc_make_redir(cmd, env);
	dup2_with_error_check(cloud[1], 1);
	close(cloud[1]);
	dup2_with_error_check(cloud[0], 0);
	close(cloud[0]);
	ft_free((void **)&cmd);
	return (value);
}
