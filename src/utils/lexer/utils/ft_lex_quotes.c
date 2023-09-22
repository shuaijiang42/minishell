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
