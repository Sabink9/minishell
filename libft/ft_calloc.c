/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:20:09 by saciurus          #+#    #+#             */
/*   Updated: 2024/11/19 15:46:31 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t e_count, size_t e_size)
{
	unsigned char	*memo;
	size_t			i;

	memo = malloc(e_count * e_size);
	if (!memo)
		return (NULL);
	i = 0;
	while (i < (e_count * e_size))
	{
		memo[i] = 0;
		i++;
	}
	return (memo);
}
