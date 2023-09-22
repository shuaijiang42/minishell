/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_make_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:02:28 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 18:02:41 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_exc_make_cmd(t_list **input)
{
	char	*result;
	char	*tmp1;
	t_list	*ptr;
	t_argument	*tmp2;

	result = NULL;
	tmp1 = NULL;
	ptr = *input;
	while (ptr)
	{
		tmp2 = (t_argument *)ptr->content;
		if (!ft_strncmp(tmp2->type, "cmd", 3))
		{
			tmp1 = result;
			result = ft_good_strjoin(result, "   ");
			ft_free((void **)&tmp1);
			tmp1 = result;
			result = ft_good_strjoin(result, tmp2->str);
			ft_free((void **)&tmp1);
		}
		ptr = ptr->next;
	}
	ft_exc_clear_content(input);
	return (result);
}


