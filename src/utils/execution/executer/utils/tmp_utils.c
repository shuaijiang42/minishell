/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:13:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:31:44 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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


