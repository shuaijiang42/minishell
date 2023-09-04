/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dollar_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:52:37 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/04 18:14:05 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ft_dollar_fill_init(t_dollar_fill *strc, int *x, t_cmd cmd, char *dst)
{
	strc->j = 4;
	strc->i = 0;
	strc->z = 0;
	strc->x = x;
	strc->dst = dst;
	strc->cmd = cmd;
}

void	ft_dollar_fill(char *str, t_cmd cmd, int *x, char *dst)
{
	int		j;
	int		i;
	int		z;
	char	*str2;
	char	*str3;
	t_list	*tmp;
	t_dollar_fill	strc;

	ft_dollar_fill_init(&strc, x, cmd, dst);
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
			str3 = ft_itoa(ft_get_error());
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
