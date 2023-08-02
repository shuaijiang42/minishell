/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 15:21:36 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/02 16:12:57 by shujiang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_flag_his
{
	tmp,
	his
}			t_flag_his;

typedef struct s_list
{
	char			*content;
	t_flag_his		flag;
	struct s_list	*prev;
	struct s_list	*next;
}	t_list;
#endif
