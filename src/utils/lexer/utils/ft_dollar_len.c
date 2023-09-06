/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dollar_len.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:50:05 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/05 15:36:03 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
	tmp = (ft_get_static())->env;
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
			str2 = ft_itoa(ft_get_error());
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

