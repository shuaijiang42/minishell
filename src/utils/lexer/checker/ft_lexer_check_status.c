/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_check_status.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 13:09:48 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/14 13:10:27 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <lexer.h>

int	dollar_delimiter(char c)
{
	if (!c || c == '$' || c == '\'' || c == '\"' \
		|| c == '|' || c == '<' || c == '>')
		return (0);
	return (1);
}

static void	ft_lex_check_quotes_util(t_command *cmd, char *str, int *i)
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

static void	ft_lex_check_quotes(t_command *cmd, char *str, int *i)
{
	if (cmd->simple_q == q_open || cmd->double_q == q_open)
		ft_lex_check_quotes_util(cmd, str, i);
	else if (str[*i] == '\'' && \
	cmd->simple_q == q_close && cmd->simple_q == q_close)
	{
		cmd->simple_q = q_open;
		cmd->status = q_open;
	}
	else if (str[*i] == '\"' && \
	cmd->simple_q == q_close && cmd->simple_q == q_close)
	{
		cmd->double_q = q_open;
		cmd->status = q_open;
	}
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
			ft_lex_check_quotes(cmd, str, i);
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
