/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 15:10:48 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/24 18:06:07 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	errors_get_next_cmd(int x, t_get_next_command *cmd, int n);
static int	init_get_next_command(char *str, t_get_next_command *cmd);
static int	get_next_return_util(t_get_next_command *cmd, int x);
static int	loop_get_next_cmd(t_get_next_command *cmd);
int			ft_get_next_command(char *str);

/*
			ft_get_next_command
		return values:
		-4 if the command is empty
		-3 if is the last or only command
		-2 if is one pipe open
		-1 if the quotes is no closed
		(other value) index of the next command in the array
*/
int	ft_get_next_command(char *str)
{
	t_get_next_command	cmd;

	if (init_get_next_command(str, &cmd) == -1)
		return (cmd.return_status);
	while (cmd.str[cmd.i] && !cmd.end)
	{
		while (cmd.status.status == q_open && cmd.str[cmd.i])
		{
			if (loop_get_next_cmd(&cmd))
				break ;
		}
		cmd.n_commands += 1;
		while (cmd.str[cmd.i] == ' ' && cmd.str[cmd.i] && !cmd.end)
			cmd.i += 1;
		if ((str[cmd.i] == '|' || str[cmd.i] == '<' || str[cmd.i] == '>'))
			return (get_next_return_util(&cmd, 1));
		if (cmd.str[cmd.i] && !cmd.end)
		{
			if ((str[cmd.i] == '|' || str[cmd.i] == '<' || str[cmd.i] == '>'))
				break ;
			cmd.status.status = q_open;
		}
	}
	return (get_next_return_util(&cmd, 0));
}

static int	errors_get_next_cmd(int x, t_get_next_command *cmd, int n)
{
	if (x == 1)
		ft_print_error("syntax error near unexpected token `|'", 258);
	else if (x == 2)
		ft_print_error("syntax error unclosed quotes", 69);
	cmd->return_status = n;
	return (-1);
}

static int	init_get_next_command(char *str, t_get_next_command *cmd)
{
	if (!str)
		return (errors_get_next_cmd(0, cmd, -1));
	cmd->str = str;
	cmd->status.dollar = funtional;
	cmd->status.simple_q = q_close;
	cmd->status.double_q = q_close;
	cmd->status.dollar = funtional;
	cmd->i = 0;
	cmd->n_commands = 0;
	cmd->command = 0;
	cmd->end = 0;
	while (cmd->str[cmd->i] == ' ')
		cmd->i += 1;
	if (cmd->str[cmd->i] == '|')
		return (errors_get_next_cmd(1, cmd, -1));
	if (str[cmd->i])
		cmd->status.status = q_open;
	else
		return (errors_get_next_cmd(0, cmd, -4));
	return (0);
}

static int	get_next_return_util(t_get_next_command *cmd, int x)
{
	if (x)
	{
		if (!cmd->str[cmd->i + 1])
			return (-2);
		else
			return (cmd->i + 1);
	}
	else
	{
		if (!cmd->str[cmd->i] && cmd->status.simple_q == q_close && \
		cmd->status.double_q == q_close)
			cmd->status.status = q_close;
		if (cmd->status.status == q_open)
			return (errors_get_next_cmd(2, cmd, -1));
		return (-3);
	}
}

static int	loop_get_next_cmd(t_get_next_command *cmd)
{
	if (ft_lexer_check_status(&cmd->status, cmd->str, &cmd->i))
	{
		cmd->end = 1;
		return (1);
	}
	return (0);
}
