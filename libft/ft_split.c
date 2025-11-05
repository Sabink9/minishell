/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:33:43 by saciurus          #+#    #+#             */
/*   Updated: 2024/11/29 17:07:28 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_word(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!s || *s == 0)
		return (0);
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

int	len_word(char const *s, char c)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	creat_tab(char const *s, char c, char **tab)
{
	int	i;
	int	j;
	int	k;

	j = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] == 0)
			return (0);
		tab[j] = (char *)malloc(sizeof(char) * (len_word(s + i, c) + 1));
		if (!tab[j])
			return (free_tab(tab), 1);
		k = 0;
		while (s[i] && s[i] != c)
			tab[j][k++] = s[i++];
		tab[j][k] = 0;
		j++;
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	int		word_count;
	char	**tab_of_tab;

	if (!s)
		return (NULL);
	word_count = count_word(s, c);
	tab_of_tab = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!tab_of_tab)
		return (NULL);
	if (creat_tab(s, c, tab_of_tab))
		return (NULL);
	tab_of_tab[word_count] = NULL;
	return (tab_of_tab);
}
