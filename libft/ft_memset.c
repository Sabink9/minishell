/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:58:32 by saciurus          #+#    #+#             */
/*   Updated: 2024/11/27 17:26:56 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int v, size_t size)
{	
	unsigned char	*p;
	unsigned char	val;
	size_t			i;

	p = (unsigned char *)dest;
	val = (unsigned char)v;
	i = 0;
	while (i < size)
	{
		p[i] = val;
		i++;
	}
	return (dest);
}
