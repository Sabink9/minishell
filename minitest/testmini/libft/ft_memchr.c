/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:53:34 by saciurus          #+#    #+#             */
/*   Updated: 2024/11/21 16:32:43 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	const unsigned char	*search;
	unsigned char		cc;

	search = (const unsigned char *) s;
	cc = (unsigned char) c;
	i = 0;
	while (i < n)
	{
		if (search[i] == cc)
			return ((void *)(&search[i]));
		i++;
	}
	return (NULL);
}
