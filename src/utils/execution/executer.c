/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/30 16:15:45 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_exc_in_redir()
{
	/*
	while (list->content == 'inp')
	
	while (list->content == 'here doc')
	*/
	return ;
}

void	ft_exc_out_redir()
{
	/*
	while (list->content == 'inp')

	while (list->content == 'here doc')
	*/
	return ;
}

char	*ft_exc_make_cmd(char *cmd)
{
	return (cmd);
	//make the entire cmd with arguments
	//	new_cmd = ft_get_cmd();
}

int		ft_exc_execution(char *cmd, char **env)
{
	char **input;

	input = ft_lexer(cmd);
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
	write(STDIN_FILENO, ">", 1);
	str = get_next_line(STDIN_FILENO);
	if (!str)
		return (-1);
	while (ft_strncmp(content->str, str, ft_strlen2(content->str)) && str)
	{
		write(STDIN_FILENO, ">", 1);
		ft_putstr_fd(str, pipes[1]);
		free(str);
		str = get_next_line(STDIN_FILENO);
		if (!str)
			return (-1);
	}
	free(str);
	close(pipes[1]);
	lex->in = pipes[0];
	return (pipes[0]);
}

int	ft_exc_open_fd(t_argument *content, t_redir type, t_exc_lex *lex)
{
	int	fd;

	fd = -1;
	if (type == inp)
	{
		printf("this is a <\n");
		content->type = ft_strdup("inp");
		fd = 0;
	}
	else if (type == hre)
	{
		return (ft_exc_here_doc(content, lex));
	}
	else if (type == trc)
	{
		printf("this is a >\n");
		fd = 0;
		content->type = ft_strdup("trc");
	}
	else if (type == apd)
	{
		printf("this is a >>\n");
		fd = 0;
		content->type = ft_strdup("apd");
	}
	return (fd);
}

t_list	*ft_exc_new_node(char *argument, t_redir type, t_exc_lex *lex)
{
	t_list	*result;
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
		//free content
		return (NULL);
	}
	result = ft_lstnew((void *)content);
	if (!result)
	{
		//free content
		return (NULL);
	}
	return (result);
}

int	ft_exc_lex_word(t_list **result, t_exc_lex *lex)
{
	char **split;
	t_list	*tmp;

	split = ft_lexer(lex->input + lex->i);
	tmp = NULL;
	tmp = ft_exc_new_node(split[0], lex->status, lex);
	ft_free_split_2(&split);
	lex->status = non;
	if (!tmp)
	{
		//ft_free_node
		return (-1);
	}
	ft_lstadd_back(result, tmp);
	return (0);
	result = NULL;
}

t_list	*ft_exc_lex_input(char *input, int std[2])
{
	t_exc_lex	lex;
	t_list		*result;

	lex.input = input;
	result = NULL;
	ft_init_exc_lex(&lex);
	if (ft_check_dup_redir(input) == -1)
		return (NULL);
	while (input[lex.i])
	{
		lex.j = ft_check_char(&lex.cmd, input[lex.i]);
		if (!lex.j || lex.j == -1)
			ft_put_status_redir(&lex, NULL);
		else
		{
			if (lex.word == INACTIVE)
			{
				if (ft_exc_lex_word(&result, &lex) == -1)
					return (NULL);
				lex.word = ACTIVE;
			}
		}
		//printf("cmd[%d]:%c[%d]\n", lex.i, input[lex.i], lex.j);
		lex.i += 1;
	}
	if (lex.status)
	{
		ft_error_exc_unexpected_token(0, 0, 0);
		return (NULL);
	}
	std[0] = lex.in;
	return (ft_lstnew(NULL));
}

int ft_exc_make_redir(char *cmd, char **env)
{
	int		result;
	int		std[2];
	t_list	*input;
	char	*new_cmd;

	std[0] = STDIN_FILENO;
	std[1] = STDOUT_FILENO;
	input = ft_exc_lex_input(cmd, std);
	if (!input)
		return (errno);
	dup2_with_error_check(std[0], STDIN_FILENO);
	dup2_with_error_check(std[1], STDOUT_FILENO);
	ft_exc_in_redir();
	ft_exc_out_redir();
	new_cmd = ft_exc_make_cmd(cmd);
	result = ft_exc_execution(new_cmd, env);
	return (result);
}

int	executer(char *cmd, char **env)
{
	int		cloud[2];
	int		value;
	
	cloud[0] = dup(0);
	cloud[1] = dup(1);
	value = ft_exc_make_redir(cmd, env);
	dup2_with_error_check(cloud[1], 1);
	close(cloud[1]);
	dup2_with_error_check(cloud[0], 0);
	close(cloud[0]);
	return (value);
}
