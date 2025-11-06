/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sab <sab@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:52 by sab               #+#    #+#             */
/*   Updated: 2025/11/06 20:47:03 by sab              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"
#include "../libft/libft.h"

void	ft_putstr_fd2(char *s)
{
	while (*s)
		write(2, s++, 1);
}

long	ft_atol(const char *str)
{
	int		i;
	long	sign;
	long	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}
void	print_args(char **argv, int i)
{
	int	first;
	int	len;

	first = 1;
	while (argv[i])
	{
		len = ft_strlen(argv[i]);
		if (!first)
			write(1, " ", 1);
		if (len > 0)
			write(1, argv[i], len);
		first = 0;
		i++;
	}
}
