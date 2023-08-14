/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 14:06:05 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/14 14:09:57 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <lexer.h>

//			ft_get_next_command
//		return values:
//		-4 if the command is empty
//		-3 if is the last or only command
//		-2 if is one pipe open
//		-1 if the quotes is no closed
//		(other value) index of the next command in the array
static int	ft_get_next_command_return_util(char *str, int i);
static int	ft_get_next_command_return_2(t_command status, char *str, int i);
static int	ft_get_next_command_return(t_command status, \
char *str, int i, int end);

int	ft_get_next_command(char *str)
{
	int				i;
	int				command;
	int				end;
	t_command		status;

	status.dollar = funtional;
	status.simple_q = q_close;
	status.double_q = q_close;
	status.dollar = funtional;
	i = 0;
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
	return (ft_get_next_command_return(status, str, i, end));
}

static int	ft_get_next_command_return_util(char *str, int i)
{
	if (!str[i + 1])
		return (-2);
	else
		return (i + 1);
}

static int	ft_get_next_command_return_2(t_command status, \
char *str, int i)
{
	if (!str[i] && status.simple_q == q_close && status.double_q == q_close)
		status.status = q_close;
	if (status.status == q_open)
	{
		ft_print_error("syntax error unclosed quotes");
		return (-1);
	}
	return (-3);
}

static int	ft_get_next_command_return(t_command status, \
char *str, int i, int end)
{
	while (str[i] && !end)
	{
		while (status.status == q_open && str[i])
		{
			if (ft_lexer_check_status(&status, str, &i))
			{
				end = 1;
				break ;
			}
		}
		while (str[i] == ' ' && str[i] && !end)
			i++;
		if ((str[i] == '|' || str[i] == '<' || str[i] == '>'))
			return (ft_get_next_command_return_util(str, i));
		if (str[i] && !end)
		{
			if ((str[i] == '|' || str[i] == '<' || str[i] == '>'))
				break ;
			status.status = q_open;
		}
	}
	return (ft_get_next_command_return_2(status, \
	str, i));
}
