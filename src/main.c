/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:34:14 by samusanc          #+#    #+#             */
/*   Updated: 2023/07/31 17:51:45 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <unistd.h>

int	main()
{
	char s;

	s = 0;
	write (1, "minishell$ ", 11);
	while (1)
	{
		while (read(0, &s, 1) > 0 && s != '\n')
			write(1, &s, 1);
		write (1, "\nminishell$ ", 12);
	}
	return (-1);
}
