/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:00:30 by saciurus          #+#    #+#             */
/*   Updated: 2024/11/27 17:25:16 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	u_c;
	int				i;

	u_c = (unsigned char)c;
	i = ft_strlen(s);
	while (i >= 0)
	{
		if ((unsigned char)s[i] == u_c)
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}
