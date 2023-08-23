/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/22 15:50:18 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/23 14:50:28 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ft_exc_in_redir()
{
	/*
	while (list->content == 'inp')
	
	while (list->content == 'here doc')
	*/
	return ;
}

void	ft_exc_out_redir()
{
	/*
	while (list->content == 'inp')

	while (list->content == 'here doc')
	*/
	return ;
}

char	*ft_exc_make_cmd(char *cmd)
{
	return (cmd);
	//make the entire cmd with arguments
	//	new_cmd = ft_get_cmd();
}

int		ft_exc_execution(char *cmd, char **env)
{
	char **input;

	input = ft_lexer(cmd);
	if (!input[0][0])
	{
		ft_print_error(": command not found", 127);
		return (127);
	}
	else
		return (ft_excuter(input, env));
}

t_list	*ft_exc_lex_input(char *cmd)
{
	/*
	int	i;
	int	j;

	i = 0;
	ft_init_cmd(&cmd);
	while (cmd[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (!j)
		{
			ft_init_cmd(&cmd);
		
		}
	}
	*/
	return (NULL);
	cmd = NULL;
}

int ft_exc_make_redir(char *cmd, char **env)
{
	int		result;
	t_list	*input;
	char	*new_cmd;

	input = ft_exc_lex_input(cmd);
	//  ft_exc_in_redir
	ft_exc_in_redir();
	//  ft_exc_out_redir
	ft_exc_out_redir();
	//	ft_exc_make_cmd
	new_cmd = ft_exc_make_cmd(cmd);
	//	ft_exc_execution
	result = ft_exc_execution(new_cmd, env);
	return (result);
}


int	executer(char *cmd, char **env)
{
	//this executer need to be improved, it does not count with the redirections
	int		cloud[2];
	int		value;
	
	cloud[0] = dup(0);
	cloud[1] = dup(1);
	value = ft_exc_make_redir(cmd, env);
	dup2_with_error_check(cloud[1], 1);
	close(cloud[1]);
	dup2_with_error_check(cloud[0], 0);
	close(cloud[0]);
	return (value);
}
