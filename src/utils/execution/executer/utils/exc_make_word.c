/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_make_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:59:04 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 17:59:14 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char *ft_exc_make_word(char *input)
{
	int		i;
	int		j;
	char	*str;
	char	*result;
	t_cmd	cmd;

	i = 0;
	j = 0;
	ft_init_cmd(&cmd);
	while (input[i])
	{
		j = ft_check_char(&cmd, input[i]);
		if (!j || j == -1)
			break ;
		i += 1;
	}
	str = ft_strdup(input);
	str[i] = '\0';
	result = ft_strdup(str);
	ft_free((void **)&str);
	return (result);
}


