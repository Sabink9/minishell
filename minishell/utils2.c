/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:52 by sab               #+#    #+#             */
/*   Updated: 2025/11/12 20:03:20 by saciurus         ###   ########.fr       */
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

int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || !((s[0] >= 'A' && s[0] <= 'Z') || (s[0] >= 'a' && s[0] <= 'z')
			|| s[0] == '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z')
				|| (s[i] >= '0' && s[i] <= '9') || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	restore_std_fds(int saved_in, int saved_out)
{
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (0);
}
