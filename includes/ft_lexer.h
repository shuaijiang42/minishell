/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:30:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/02 22:55:25 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_LEXER_H
# define FT_LEXER_H

typedef enum e_bool
{
	false = 0,
	true = 1
}			t_bool;

typedef struct s_tokens
{
	t_bool	simple_cuotes;
	t_bool	double_cuotes;
	t_bool	dollar;
}	t_tokens;

char	**ft_lexer(char **argv);

#endif
