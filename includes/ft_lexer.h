/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:30:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/04 20:35:59 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_LEXER_H
# define FT_LEXER_H

typedef enum e_bool{
	false = 0,
	true = 1
}			t_bool;

typedef enum e_quotes{
	q_close = 0,
	q_open = 1
}			t_quotes;

typedef enum e_dollar{
	not_funtional = 0,
	funtional = 1
}			t_dollar;

//el dollar sing funciona solo si las commillas son dobles
typedef struct t_command{
	t_quotes	simple_q;
	t_quotes	double_q;
	t_dollar	dollar;
	t_quotes	status;
}				t_command;

char	**ft_lexer(char **arguments);
#endif
