/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:54:15 by saciurus          #+#    #+#             */
/*   Updated: 2024/11/21 16:47:57 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t size)
{
	const unsigned char	*s;
	unsigned char		*d;
	size_t				i;

	if (src == NULL && dest == NULL)
		return (NULL);
	s = (const unsigned char *) src;
	d = (unsigned char *) dest;
	if (s < d && d < s + size)
	{
		while (size--)
			d[size] = s[size];
	}
	else
	{
		i = 0;
		while (i < size)
		{
			d[i] = s[i];
			i++;
		}
	}
	return (dest);
}
