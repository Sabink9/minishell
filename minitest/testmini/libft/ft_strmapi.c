/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 19:02:23 by saciurus          #+#    #+#             */
/*   Updated: 2024/11/19 14:28:15 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	i;
	char	*mapi;

	mapi = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!mapi)
		return (NULL);
	i = 0;
	while (s[i])
	{
		mapi[i] = (*f)(i, s[i]);
		i++;
	}
	mapi[i] = 0;
	return (mapi);
}
