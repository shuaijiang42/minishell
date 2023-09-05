/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dollar_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:52:37 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/05 15:36:25 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ft_dollar_fill_init(t_dollar_fill *strc, int *x, char *dst)
{
	strc->j = 4;
	strc->i = 0;
	strc->z = 0;
	strc->x = x;
	strc->dst = dst;
	strc->tmp = (ft_get_static())->env;
}

static int	ft_dollar_fill_util(t_dollar_fill *strc)
{
	if (!strc->tmp)
	{
		if (strc->str2[0] == '?')
		{
			strc->str3 = ft_itoa(ft_get_error());
			ft_strlcpy(strc->dst, strc->str3, ft_strlen(strc->str3) + 1);
			strc->i = ft_strlen(strc->str3);
			strc->x[0] += strc->i;
			ft_free((void **)&strc->str3);
			ft_free((void **)&strc->str2);
			return (0);
		}
		else
			strc->dst[0] = '\0';
		ft_free((void **)&strc->str2);
		return (0);
	}
	return (1);
}

static int	ft_dollar_fill_util_2(t_dollar_fill *strc, char *str)
{
	if (strc->i == 1)
	{
		strc->dst[0] = '$';
		strc->x[0] += 1;
		return (0);
	}
	strc->str2 = malloc(sizeof(char) * (strc->i + 1));
	if (!strc->str2)
		return (0);
	ft_strlcpy(strc->str2, str, strc->i);
	strc->str2[strc->i] = '\0';
	return (1);
}

static int	ft_dollar_checkin(t_dollar_fill *strc, char *str, t_cmd	*cmd)
{
	if (!str)
		return (0);
	if (!str[0])
	{
		strc->dst[0] = '$';
		strc->x[0] += 1;
		return (0);
	}
	strc->j = ft_check_char(cmd, str[strc->i++]);
	return (1);
}

void	ft_dollar_fill(char *str, t_cmd cmd, int *x, char *dst)
{
	t_dollar_fill	strc;

	ft_dollar_fill_init(&strc, x, dst);
	if (!ft_dollar_checkin(&strc, str, &cmd))
		return ;
	while (strc.j == 3)
		strc.j = ft_check_char(&cmd, str[strc.i++]);
	if (!ft_dollar_fill_util_2(&strc, str))
		return ;
	while (strc.tmp)
	{
		if (!ft_strncmp((char *)strc.tmp->content, strc.str2, ft_strlen(strc.str2)))
			break ;
		strc.tmp = strc.tmp->next;
	}
	if(!ft_dollar_fill_util(&strc))
		return ;
	strc.str3 = (char *)strc.tmp->content + strc.i;
	ft_strlcpy(dst, strc.str3, ft_strlen(strc.str3) + 1);
	*x += ft_strlen(strc.str3);
	return ;
}
