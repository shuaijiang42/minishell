/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exc_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:14:28 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 16:14:45 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int		ft_exc_execution(char *cmd, char **env)
{
	char **input;

	if (!cmd)
		return (0);
	input = ft_lexer(cmd);
	ft_free((void **)&cmd);
	if (!*input)
		return ((int)ft_free_split_2(&input) + 258);
	if (!input[0][0])
	{
		ft_print_error(": command not found", 127);
		return (127);
	}
	else
		return (ft_excuter(input, env));
}
