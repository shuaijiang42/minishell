/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 10:43:39 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/14 15:20:46 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//	1 si no es imprimible, 2 si es imprimible, 0 si es espacio y -1 es que ha acabado
#include <lexer.h>
//+++++++++++++++++++++++++LEXER CORE++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++$$+++++++++++++++++++++++++++++++++++++++++++++++++++

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
		ft_free((void **)&str2);
		return ;
	}
	str3 = (char *)tmp->content + i;
	ft_strlcpy(dst, str3, ft_strlen(str3) + 1);
	*x += ft_strlen(str3);
	return ;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//	this funtions count how many numbers need per argument
char **ft_lexer(char *str)
{
	char			**result;
	int				len;
	int				i;

	if (ft_check_argument(str) == -1)
		return (NULL);
	len = ft_lexer_len_n_arguments(str);
	result = malloc(sizeof(char *) * (len + 1));
	if (!result)
		return (NULL);
	result[len] = NULL;
	ft_alloc_parse_result(&result, str, len);
	return (result);
	str = NULL;
	i = 0;
}
