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
