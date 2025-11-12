/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:49 by sab               #+#    #+#             */
/*   Updated: 2025/11/12 20:03:15 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"
#include "../libft/libft.h"

char	*strjoin_char_free(char *s, char c)
{
	int		len;
	char	*res;
	int		i;

	len = 0;
	while (s && s[len])
		len++;
	res = malloc(len + 2);
	if (!res)
		return (free(s), NULL);
	i = -1;
	while (++i < len)
		res[i] = s[i];
	res[i++] = c;
	res[i] = '\0';
	free(s);
	return (res);
}

char	*strjoin_free(char *s1, char *s2)
{
	int		len1;
	int		len2;
	char	*res;
	int		i;
	int		j;

	len1 = 0;
	while (s1 && s1[len1])
		len1++;
	len2 = 0;
	while (s2 && s2[len2])
		len2++;
	res = malloc(len1 + len2 + 1);
	if (!res)
		return (free(s1), NULL);
	i = -1;
	while (++i < len1)
		res[i] = s1[i];
	j = 0;
	while (j < len2)
		res[i++] = s2[j++];
	res[i] = '\0';
	free(s1);
	return (res);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*res;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	res = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		res[i + j] = s2[j];
		j++;
	}
	res[i + j] = '\0';
	return (res);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	skip_args(char **args, int *i)
{
	args[*i] = NULL;
	args[*i + 1] = NULL;
	*i += 2;
}
