/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hola.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 15:21:10 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/29 15:26:10 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int	main()
{
	int	pipex[2];
	char	str[100];
	
	pipe(pipex);
	write(pipex[1], "hola mundo\n", 11);
	read(pipex[0], &str, 50);
	printf("str:%s|\n", str);
}
