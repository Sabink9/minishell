#include <stdlib.h>
#include "../libft/libft.h"

static int	ft_word_len(const char *s, int *i)
{
	int		len = 0;
	char	quote = 0;

	while (s[*i] == ' ')
		(*i)++;
	if (s[*i] == '\'' || s[*i] == '"')
	{
		quote = s[(*i)++];
		while (s[*i] && s[*i] != quote)
		{
			len++;
			(*i)++;
		}
		if (s[*i] == quote)
			(*i)++;
	}
	else
	{
		while (s[*i] && s[*i] != ' ')
		{
			len++;
			(*i)++;
		}
	}
	return (len);
}

static char	*ft_word_dup(const char *s, int start, int len)
{
	char	*res;
	int		j;

	res = malloc(len + 1);
	if (!res)
		return (NULL);
	j = 0;
	while (j < len)
	{
		res[j] = s[start + j];
		j++;
	}
	res[j] = '\0';
	return (res);
}

static int	ft_count_words(const char *s)
{
	int	i = 0;
	int	count = 0;

	while (s[i])
	{
		if (s[i] != ' ')
		{
			ft_word_len(s, &i);
			count++;
		}
		else
			i++;
	}
	return (count);
}

char	**ft_split(const char *s)
{
	int		i = 0;
	int		j = 0;
	int		len;
	int		start;
	int		wc;
	char	**res;

	if (!s)
		return (NULL);
	wc = ft_count_words(s);
	res = malloc(sizeof(char *) * (wc + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (j < wc)
	{
		while (s[i] == ' ')
			i++;
		start = i;
		len = ft_word_len(s, &i);
		res[j] = ft_word_dup(s, start + (s[start] == '\'' || s[start] == '"'), len);
		j++;
	}
	res[j] = NULL;
	return (res);
}
void	free_split(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
}

