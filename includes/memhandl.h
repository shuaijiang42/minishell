/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memhandl.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shujiang <shujiang@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 16:30:42 by shujiang          #+#    #+#             */
/*   Updated: 2023/08/31 14:16:51 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMHANDL_H
# define MEMHANDL_H
# include <libft.h>

void	*ft_free(void **str);
void	*ft_print_error(char *str, int error);
void	leaks();
#endif
