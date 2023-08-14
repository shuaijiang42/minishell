/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 11:12:02 by samusanc          #+#    #+#             */
/*   Updated: 2023/08/14 14:51:31 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <lexer.h>

static void	ft_lexer_len_argument_loop(char *str, t_cmd *cmd, int *i, int *len)
{
	int	j;

	j = ft_check_char(cmd, str[*i]);
	if (j == 2)
		*len += 1;
	if (j == 4)
		*len += ft_dollar_len(str + *i + 1, *cmd);
	*i += 1;
}

int	ft_lexer_len_argument(char *str)
{
	t_cmd			cmd;
	int				len;
	int				i;
	int				j;

	len = 0;
	i = 0;
	j = 0;
	ft_init_cmd(&cmd);
	while (!j && str[i])
		ft_lexer_len_argument_loop(str, &cmd, &i, &len);
	if (str[i] && j >= 0)
	{
		while (str[i] && j > 0)
			ft_lexer_len_argument_loop(str, &cmd, &i, &len);
	}
	return (len);
}

static int	ft_lexer_len_n_arguments_loop(char *str, t_cmd *cmd, int *i, int *x)
{		
	int	j;
	int	len;

	len = 0;
	j = ft_check_char(cmd, str[*i]);
	if (j > 0)
	{
		len = 1;
		*x = 1;
	}
	*i += 1;
	return (len);
}

int	ft_lexer_len_n_arguments(char *str)
{
	t_cmd			cmd;
	int				len;
	int				i;
	int				j;
	int				x;

	len = 0;
	i = 0;
	j = 0;
	x = 0;
	ft_init_cmd(&cmd);
	while (str[i] && !j && j != -1)
		len += ft_lexer_len_n_arguments_loop(str, &cmd, &i, &x);
	while (str[i] && j >= 0)
	{
		while (str[i] && j > 0 && j != -1)
			len += ft_lexer_len_n_arguments_loop(str, &cmd, &i, &x);
		x = 0;
		ft_init_cmd(&cmd);
		while (str[i] && !j && j != -1)
			j = ft_check_char(&cmd, str[i++]);
	}
	return (len);
}

void	ft_lexer_fill_str(char *str, char **str2)
{
	t_cmd			cmd;
	int				len;
	int				i;
	int				j;
	int				x;

	len = 0;
	i = 0;
	j = 0;
	x = 0;
	ft_init_cmd(&cmd);
	while (!j && str[i])
	{
		j = ft_check_char(&cmd, str[i]);
		if (j == 2)
			str2[0][x++] = str[i];
		if (j == 4)
			ft_dollar_fill(str + i + 1, cmd, &x, str2[0] + x);
		i++;
	}
	if (str[i] && j >= 0)
	{
		while (str[i] && j > 0)
		{
			j = ft_check_char(&cmd, str[i]);
			if (j == 2)
				str2[0][x++] = str[i];
			if (j == 4)
				ft_dollar_fill(str + i + 1, cmd, &x, str2[0] + x);
			i++;
		}
	}
}
//debbug printf:
//printf("[%d] = '%c', j = %d\n", i, str[i], j);

int	ft_lexer_get_next_argument(char *str)
{
	t_cmd			cmd;
	int				len;
	int				i;
	int				j;

	len = 0;
	i = 0;
	j = 0;
	ft_init_cmd(&cmd);
	while (str[i] && !j)
		j = ft_check_char(&cmd, str[i++]);
	while (str[i] && j >= 0)
	{
		while (str[i] && j > 0)
			j = ft_check_char(&cmd, str[i++]);
		return (i);
	}
	return (ft_strlen(str));
}

void	ft_alloc_parse_result(char ***result_ptr, char *str, int len)
{
	char	**result;
	int		i;
	int		arg_len;
	char	*str2;
	int		x;

	i = 0;
	x = 0;
	result = *result_ptr;
	arg_len = 0;
	while (len)
	{
		arg_len = ft_lexer_len_argument(str);
		str2 = ft_calloc(sizeof(char), arg_len + 1);
		if (!str2)
			return ;
		str2[arg_len] = '\0';
		ft_lexer_fill_str(str, &str2);
		result[x++] = str2;
		i = ft_lexer_get_next_argument(str);
		str += i;
		len--;
	}
}
