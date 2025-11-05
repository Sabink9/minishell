/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 19:01:28 by saciurus          #+#    #+#             */
/*   Updated: 2024/11/27 17:24:09 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_intlen(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		n = -n;
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

void	create_tab(int n, int size, int signe, char *tab)
{
	int	count;

	count = size - 1;
	while (count >= 0)
	{
			tab[count] = (n % 10) + '0';
		n /= 10;
		count--;
	}
	if (signe == 1)
		tab[0] = '-';
}

char	*ft_itoa(int n)
{
	char	*ita;
	int		n_len;
	int		is_neg;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	is_neg = 0;
	if (n < 0)
	{
		is_neg = 1;
		n = -n;
	}
	n_len = ft_intlen(n) + is_neg;
	ita = (char *)malloc(sizeof (char) * (n_len + 1));
	if (!ita)
		return (NULL);
	create_tab(n, n_len, is_neg, ita);
	ita[n_len] = 0;
	return (ita);
}
