/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/22 16:04:40 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_exc_in_redir()
{

}

void	ft_exc_out_redir()
{

}

char	*ft_exc_make_cmd()
{

}

int		ft_exc_execution()
{
	input = ft_lexer(new_cmd);
	if (!input[0][0])
	{
		ft_print_error(": command not found", 127);
		value = 127;
	}
	else
		value = ft_excuter(input, env);
}

void ft_exc_make_redir()
{
	//  ft_exc_in_redir
	{
		while (list->content == 'inp')
		
		while (list->content == 'here doc')

	}
	//  ft_exc_out_redir
	{

		while (list->content == 'inp')

		while (list->content == 'here doc')

	}
	//	ft_exc_make_cmd
	{
		//make the entire cmd with arguments
	//	new_cmd = ft_get_cmd();
	}
	//	ft_exc_execution
}

t_list	ft_exc_lex_input(char *cmd)
{

}

int	executer(char *cmd, char **env)
{
	//this executer need to be improved, it does not count with the redirections
	char **input;
	char	*new_cmd;


	//ft_exc_lex_input
	{
		//lex the list
		//lex cmd list;
	}
	//this code belong to other place, i think the execute cmd, so maybe u can try to move it there?
	//-----------------------------------------------------------//
	int	cloud[2];
	int	value;
	
	cloud[0] = dup(0);
	cloud[1] = dup(1);
	ft_exc_make_redir();
	dup2_with_error_check(cloud[1], 1);
	close(cloud[1]);
	dup2_with_error_check(cloud[0], 0);
	close(cloud[0]);
	return (value);
}
