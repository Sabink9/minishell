/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:54:13 by saciurus          #+#    #+#             */
/*   Updated: 2024/11/27 17:24:41 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int				i;
	unsigned char	u_c;

	u_c = (unsigned char)c;
	i = 0;
	while (s[i])
	{
		if ((unsigned char)s[i] == u_c)
		{
			return ((char *)&s[i]);
		}
			i++;
	}
	if (u_c == 0)
		return ((char *)&s[i]);
	return (NULL);
}
