/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 16:11:58 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/14 16:18:08 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <lexer.h>

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

int	dollar_delimiter(char c)
{
	if (!c || c == '$' || c == '\'' || c == '\"' \
		|| c == '|' || c == '<' || c == '>')
		return (0);
	return (1);
}

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
	while (str[i] == ' ')
		i++;
	if (str[i] == '|')
	{
		ft_print_error("syntax error near unexpected token `|'");
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
		ft_print_error("syntax error unclosed quotes");
		return (-1);
	}
	return (-3);
}
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

void	*ft_free_split_2(char ***split)
{
	int i;

	i = 0;
	while (split[0][i])
	{
		if (split[0][i++])
			ft_free((void *)&split[0][i]);
	}
	ft_free((void *)&split[0]);
	*split = NULL;
	return (NULL);
}

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

void	ft_init_cmd(t_cmd *cmd)
{
	cmd->quotes = no_q;
	cmd->dollar_status = q_close;
	cmd->status = q_close;
	cmd->spaces = 1;
}

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
	tmp = (ft_get_static())->env_cpy;
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
			str2 = ft_itoa(42);
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

void	ft_dollar_fill(char *str, t_cmd cmd, int *x, char *dst)
{
	int	j;
	int	i;
	int	z;
	char	*str2;
	char	*str3;
	t_list	*tmp;

	j = 4;
	i = 0;
	z = 0;
	if (!str)
		return ;
	if (!str[0])
	{
		dst[0] = '$';
		*x += 1;
		return ;
	}
	j = ft_check_char(&cmd, str[i++]);
	while (j == 3)
		j = ft_check_char(&cmd, str[i++]);
	if (i == 1)
	{
		dst[0] = '$';
		*x += 1;
		return ;
	}
	str2 = malloc(sizeof(char) * (i + 1));
	if (!str2)
		return ;
	ft_strlcpy(str2, str, i);
	str2[i] = '\0';
	tmp = (ft_get_static())->env_cpy;
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
			str3 = ft_itoa(42);
			ft_strlcpy(dst, str3, ft_strlen(str3) + 1);
			i = ft_strlen(str3);
			*x += i;
			ft_free((void **)&str3);
			ft_free((void **)&str2);
			return ;
		}
		else
			dst[0] = '\0';
		ft_free((void **)&str2);
		return ;
	}
	str3 = (char *)tmp->content + i;
	ft_strlcpy(dst, str3, ft_strlen(str3) + 1);
	*x += ft_strlen(str3);
	return ;
}

//	this funtions count how many numbers need per argument
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
			len++;
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
				len++;
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
			j = ft_check_char(&cmd, str[i++]);
	}
	return (len);
}

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
		str2[arg_len] = '\0';
		ft_lexer_fill_str(str, &str2);
		result[x++] = str2;
		i = ft_lexer_get_next_argument(str);
		str += i;
		len--;
	}
	return ;
	str = NULL;
	len = 0;
}

char **ft_lexer(char *str)
{
	char			**result;
	//t_cmd			cmd;
	int				len;
	int				i;

	if (ft_check_argument(str) == -1)
		return (NULL);
	len = ft_lexer_len_n_arguments(str);
	// y si len da 0 que se hace???
	result = malloc(sizeof(char *) * (len + 1));
	if (!result)
		return (NULL);
	result[len] = NULL;
	ft_alloc_parse_result(&result, str, len);
	return (result);
	str = NULL;
	i = 0;
}

