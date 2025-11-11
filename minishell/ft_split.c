/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:16 by sab               #+#    #+#             */
/*   Updated: 2025/11/11 11:50:20 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

/* duplique brut le segment [start, end) SANS enlever les quotes */
static char	*dup_raw(const char *s, int start, int end)
{
	char	*out;
	int		i;
	int		k;

	out = malloc(end - start + 1);
	i = start;
	k = 0;
	if (!out)
		return (NULL);
	while (i < end)
		out[k++] = s[i++];
	out[k] = '\0';
	return (out);
}

/* lit un token (mot avec quotes, ou '|' ) et l'ajoute dans tab */
static int	push_tok(char **tab, int *count, char *tok)
{
	if (!tok)
		return (0);
	tab[*count] = tok;
	(*count)++;
	return (1);
}

static int	scan_tok(const char *s, int i, int *q)
{
	while (s[i])
	{
		if (*q == 0 && (s[i] == ' ' || s[i] == '\t' || s[i] == '|'))
			break ;
		if (s[i] == '\'' || s[i] == '\"')
		{
			if (*q == 0)
				*q = s[i];
			else if (*q == s[i])
				*q = 0;
		}
		i++;
	}
	return (i);
}

static int	next_token(char *s, int *i, char **tab, int *count)
{
	int	start;
	int	q;

	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
	if (!s[*i])
		return (0);
	if (s[*i] == '|')
	{
		if (!push_tok(tab, count, dup_raw(s, *i, *i + 1)))
			return (-1);
		(*i)++;
		return (1);
	}
	start = *i;
	q = 0;
	*i = scan_tok(s, *i, &q);
	if (!push_tok(tab, count, dup_raw(s, start, *i)))
		return (-1);
	return (1);
}

char	**ft_split(char *s)
{
	int		i;
	int		r;
	int		count;
	char	**tab;

	if (!s)
		return (NULL);
	tab = (char **)malloc(sizeof(char *) * ((int)ft_strlen(s) / 2 + 2));
	if (!tab)
		return (NULL);
	i = 0;
	count = 0;
	while (1)
	{
		r = next_token(s, &i, tab, &count);
		if (r == -1)
			return (free_partial(tab, count), NULL);
		if (r == 0)
			break ;
	}
	tab[count] = NULL;
	return (tab);
}
