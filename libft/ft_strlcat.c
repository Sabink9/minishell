/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:13:05 by saciurus          #+#    #+#             */
/*   Updated: 2024/11/27 17:24:48 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t n)
{
	size_t	i;
	size_t	dest_len;
	size_t	src_len;

	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	if (n == 0 || n <= dest_len)
		return (src_len + n);
	i = 0;
	while (src[i] && dest_len + i < n - 1)
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[i + dest_len] = 0;
	return (src_len + dest_len);
}
