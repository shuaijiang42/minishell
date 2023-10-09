/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utiles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 17:46:02 by shujiang          #+#    #+#             */
/*   Updated: 2023/10/09 12:32:49 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	error_parsing_export(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier'\n", STDERR_FILENO);
	errno = 1;
	return (0);
}

int	error_ft_parsing(char *str)
{
	error_parsing_export(str);
	if (*str == '-')
		errno = 3;
	return (0);
}

/* Each argument for the export cmd 
can only contain alphanumeric character or '_' or '='
and the numeric characters 
and '=' can't be at the beginning of the argument*/
int	ft_parsing(char *str)
{
	int	i;
	int	vocal;

	i = 0;
	vocal = 0;
	if (*str == '=' || *str == '-')
		return (error_ft_parsing(str));
	while (str[i] && str[i] != '=')
	{
		if (str[i] == ' ')
			return (error_parsing_export(str));
		else if (!ft_isalpha(str[i]))
		{
			if (ft_isdigit(str[i]) && !vocal)
				return (error_parsing_export(str));
			else if (str[i] != '_')
				return (error_parsing_export(str));
		}
		else if (ft_isalpha(str[i]))
			vocal += 1;
		i++;
	}
	return (1);
}
