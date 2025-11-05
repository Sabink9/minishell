/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 20:06:46 by saciurus          #+#    #+#             */
/*   Updated: 2024/11/15 12:14:01 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ts;
	size_t			i;

	ts = (unsigned char *) s;
	i = 0;
	while (i < n)
	{
		ts[i] = 0;
		i++;
	}
}
