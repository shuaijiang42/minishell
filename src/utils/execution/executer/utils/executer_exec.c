/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 18:03:30 by samusanc          #+#    #+#             */
/*   Updated: 2023/09/22 18:03:46 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_executer_exec(t_input *input, char **env)
{
	int	result;

	result = 0;

	dup2_with_error_check(input->in, STDIN_FILENO);
	close(input->in);
	dup2_with_error_check(input->out, STDOUT_FILENO);
	close(input->out);
	result = ft_exc_execution(input->cmd, env);
	return (result);
}


