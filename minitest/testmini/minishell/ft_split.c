#include "../libft/libft.h"
#include "mini.h"

/* saute espaces/tabs */
// static int	is_space(int c)
// {
// 	return (c == ' ' || c == '\t');
// }

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

/* Split: ignore espaces hors-quotes, conserve les quotes dans les tokens */
/* Split: ignore espaces hors-quotes, conserve les quotes dans les tokens */
static void	free_partial(char **tab, int count)
{
	int	k;

	k = 0;
	while (k < count)
	{
		free(tab[k]);
		k++;
	}
	free(tab);
}

char	**ft_split(char *s)
{
	int		i;
	int		start;
	int		q;
	int		count;
	int		cap;
	char	**tab;
	char	c;

	if (!s)
		return (NULL);
	cap = (int)ft_strlen(s) / 2 + 2;
	tab = (char **)malloc(sizeof(char *) * cap);
	if (!tab)
		return (NULL);
	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && (s[i] == ' ' || s[i] == '\t'))
			i++;
		if (!s[i])
			break ;
		start = i;
		q = 0;
		while (s[i])
		{
			c = s[i];
			if (c == '\'' || c == '\"')
			{
				if (q == 0)
					q = c;
				else if (q == c)
					q = 0;
				i++;
				continue ;
			}
			if (q == 0 && c == '|')
			{
				if (i > start)
				{
					tab[count] = dup_raw(s, start, i);
					if (!tab[count])
					{
						free_partial(tab, count);
						return (NULL);
					}
					count++;
				}
				tab[count] = dup_raw(s, i, i + 1); /* "|" */
				if (!tab[count])
				{
					free_partial(tab, count);
					return (NULL);
				}
				count++;
				i++;
				start = i;
				break ;
			}
			if (q == 0 && (c == ' ' || c == '\t'))
				break ;
			i++;
		}
		if (start < i)
		{
			tab[count] = dup_raw(s, start, i);
			if (!tab[count])
			{
				free_partial(tab, count);
				return (NULL);
			}
			count++;
		}
	}
	tab[count] = NULL;
	return (tab);
}

void	free_split(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}
