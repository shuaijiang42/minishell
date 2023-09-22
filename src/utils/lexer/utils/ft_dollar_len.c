/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dollar_len.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:50:05 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/18 16:43:26 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	dollar_return(t_dollar_len *dollar, int n)
{
	dollar->return_value = n;
	return (-1);
}

static void	init_init_dollar_len(char *str, t_cmd cmd, \
t_dollar_len *dollar)
{
	dollar->j = 4;
	dollar->i = 0;
	dollar->cmd = cmd;
	dollar->str = str;
	dollar->j = ft_check_char(&dollar->cmd, dollar->str[dollar->i]);
	dollar->i += 1;
	dollar->return_value = 0;
	dollar->tmp = (ft_get_static())->env;
	dollar->str2 = NULL;
}

static int	init_dollar_len(char *str, t_cmd cmd, t_dollar_len *dollar)
{
	if (!str)
		return (dollar_return(dollar, 0));
	if (!str[0])
		return (dollar_return(dollar, 1));
	init_init_dollar_len(str, cmd, dollar);
	while (dollar->j == 3)
	{
		dollar->j = ft_check_char(&dollar->cmd, dollar->str[dollar->i]);
		dollar->i += 1;
	}
	if (dollar->i == 1)
		return (dollar_return(dollar, 1));
	dollar->str2 = malloc(sizeof(char) * (dollar->i + 1));
	if (!dollar->str2)
		return (dollar_return(dollar, 0));
	ft_strlcpy(dollar->str2, dollar->str, dollar->i);
	dollar->str2[dollar->i] = '\0';
	return (0);
}

static int	dollar_flags(t_dollar_len *dollar)
{
	if (dollar->str2[0] == '?')
	{
		ft_free((void **)&dollar->str2);
		dollar->str2 = ft_itoa(ft_get_error());
		dollar->i = ft_strlen(dollar->str2);
		ft_free((void **)&dollar->str2);
		return (dollar->i);
	}
	ft_free((void **)&dollar->str2);
	return (1);
}

int	ft_dollar_len(char *str, t_cmd cmd)
{
	t_dollar_len	dollar;

	if (init_dollar_len(str, cmd, &dollar) == -1)
		return (dollar.return_value);
	while (dollar.tmp)
	{
		if (!ft_strncmp((char *)dollar.tmp->content, \
		dollar.str2, ft_strlen(dollar.str2)))
			break ;
		dollar.tmp = dollar.tmp->next;
	}
	if (!dollar.tmp)
		return (dollar_flags(&dollar));
	ft_free((void **)&dollar.str2);
	return (ft_strlen(((char *)dollar.tmp->content + dollar.i)) + 1);
}
