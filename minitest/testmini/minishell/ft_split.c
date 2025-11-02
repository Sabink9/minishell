#include "../libft/libft.h"
#include "mini.h"

/* saute espaces/tabs */
static int	is_space(int c)
{
	return (c == ' ' || c == '\t');
}

/* duplique brut le segment [start, end) SANS enlever les quotes */
static char	*dup_raw(const char *s, int start, int end)
{
	char	*out;
	int		i;
	int	k;

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
char	**ft_split(char *s)
{
	int	i;
	int	q;
	int	count;
	int	cap;
	int	start;
	int	j;

	i = 0;
	q = 0;
	count = 0;
	char **tab, c;
	if (!s)
		return (NULL);
	cap = (int)ft_strlen((char *)s) / 2 + 2;
	tab = malloc(sizeof(char *) * cap);
	if (!tab)
		return (NULL);
	while (s[i])
	{
		while (s[i] && is_space(s[i]))
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
				if (!q)
					q = c;
				else if (q == c)
					q = 0;
				i++;
				continue ;
			}
			if (!q && is_space(c))
				break ;
			i++;
		}
		tab[count] = dup_raw(s, start, i);
		if (!tab[count])
		{
			j = 0;
			while (j < count)
				free(tab[j++]);
			free(tab);
			return (NULL);
		}
		count++;
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
