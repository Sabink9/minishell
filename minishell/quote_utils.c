/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:56:19 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/10 18:29:03 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

static int	should_write_quote(char *q, char c)
{
	if (*q == 0)
	{
		*q = c;
		return (0);
	}
	if (*q == c)
	{
		*q = 0;
		return (0);
	}
	return (1);
}

/* copie s dans out en supprimant les quotes inutiles */
static void	copy_unquoted(const char *s, char *out)
{
	int		i;
	int		k;
	char	q;

	i = 0;
	k = 0;
	q = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			if (should_write_quote(&q, s[i]))
				out[k++] = s[i];
		}
		else if (s[i] == (char)-1)
			out[k++] = '$';
		else
			out[k++] = s[i];
		i++;
	}
	out[k] = '\0';
}

char	*unquote_token(const char *s)
{
	char	*out;

	if (!s)
		return (NULL);
	out = malloc(ft_strlen(s) + 1);
	if (!out)
		return (NULL);
	copy_unquoted(s, out);
	return (out);
}

/* compte combien d'arguments non nuls sont présents */
static int	count_valid_args(char **args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (args && args[i])
	{
		if (args[i] != NULL)
			count++;
		i++;
	}
	return (count);
}

/* duplique args en supprimant les NULL et les quotes */
char	**compact_argv(char **args)
{
	int		i;
	int		k;
	int		keep;
	char	**out;

	keep = count_valid_args(args);
	out = malloc(sizeof(char *) * (keep + 1));
	if (!out)
		return (NULL);
	i = 0;
	k = 0;
	while (args && args[i])
	{
		if (args[i] != NULL)
			out[k++] = unquote_token(args[i]);
		i++;
	}
	out[k] = NULL;
	return (out);
}
