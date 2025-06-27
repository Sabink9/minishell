#include "mini.h"

int	count_word(const char *s, char c)
{
	int	i = 0;
	int	count = 0;
	char	quote = 0;

	if (!s || *s == 0)
		return (0);
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] == '\0')
			break;
		count++;
		if (s[i] == '\'' || s[i] == '"')
		{
			quote = s[i++];
			while (s[i] && s[i] != quote)
				i++;
			if (s[i] == quote)
				i++;
		}
		else
		{
			while (s[i] && s[i] != c && s[i] != '\'' && s[i] != '"')
				i++;
		}
	}
	return (count);
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

static int	len_word_quote(const char *s, char c)
{
	int		i = 0;
	char	quote;

	if (s[i] == '\'' || s[i] == '"')
	{
		quote = s[i++];
		while (s[i] && s[i] != quote)
			i++;
		if (s[i] == quote)
			i++;
	}
	else
	{
		while (s[i] && s[i] != c && s[i] != '\'' && s[i] != '"')
			i++;
	}
	return (i);
}

int	creat_tab(const char *s, char c, char **tab)
{
	int		i = 0;
	int		j = 0;
	int		k;
	char	quote;

	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] == '\0')
			break;

		k = 0;
		int len = len_word_quote(s + i, c);
		tab[j] = malloc(len + 1);
		if (!tab[j])
			return (free_tab(tab), 1);

		if (s[i] == '\'' || s[i] == '"')
		{
			quote = s[i++];
			while (s[i] && s[i] != quote)
				tab[j][k++] = s[i++];
			if (s[i] == quote)
				i++;
		}
		else
		{
			while (s[i] && s[i] != c && s[i] != '\'' && s[i] != '"')
				tab[j][k++] = s[i++];
		}
		tab[j][k] = '\0';
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