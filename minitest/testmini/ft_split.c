#include <stdlib.h>

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_escaped(char *string, int i)
{
	int	count;

	count = 0;
	i--;
	while (i >= 0 && string[i] == '\\')
	{
		count++;
		i--;
	}
	return (count % 2);
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

int	count_words(char *string, char sep)
{
	int	i;
	int	count;
	int	in_word;
	char	quote;

	i = 0;
	count = 0;
	in_word = 0;
	quote = 0;
	while (string[i])
	{
		if (is_quote(string[i]) && !is_escaped(string, i))
		{
			if (quote == 0)
				quote = string[i];
			else if (quote == string[i])
				quote = 0;
		}
		if (string[i] != sep && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (string[i] == sep && quote == 0)
			in_word = 0;
		i++;
	}
	return (count);
}

int	len_word_loop(char *string, char sep, int *i, char *quote)
{
	int	len;

	len = 0;
	while (string[*i])
	{
		if (is_quote(string[*i]) && !is_escaped(string, *i))
		{
			if (*quote == 0)
				*quote = string[*i];
			else if (*quote == string[*i])
				*quote = 0;
		}
		else if (string[*i] == sep && *quote == 0)
			break ;
		len++;
		(*i)++;
	}
	return (len);
}

int	len_word(char *string, char sep, int i)
{
	char	quote;
	int	tmp;

	quote = 0;
	tmp = i;
	return (len_word_loop(string, sep, &tmp, &quote));
}

int	copy_word_loop(char *dst, char *string, int j, char *quote)
{
	int	i;

	i = 0;
	while (string[j])
	{
		if (is_quote(string[j]) && !is_escaped(string, j))
		{
			if (*quote == 0)
				*quote = string[j];
			else if (*quote == string[j])
				*quote = 0;
			j++;
		}
		else if (string[j] == '\\' && string[j + 1])
		{
			dst[i++] = string[j++];
			dst[i++] = string[j++];
		}
		else if (string[j] == ' ' && *quote == 0)
			break ;
		else
			dst[i++] = string[j++];
	}
	dst[i] = '\0';
	return (j);
}

int	copy_word(char *dst, char *string, int start)
{
	int		j;
	char	quote;

	j = start;
	quote = 0;
	return (copy_word_loop(dst, string, j, &quote));
}

int	create_tab(char **split, char *string, char sep)
{
	int	i;
	int	k;
	int	word_len;
	int	offset;

	i = 0;
	k = 0;
	while (string[i])
	{
		while (string[i] == sep)
			i++;
		if (!string[i])
			break ;
		word_len = len_word(string, sep, i);
		split[k] = malloc(sizeof(char) * (word_len + 1));
		if (!split[k])
		{
			free_tab(split);
			return (0);
		}
		offset = copy_word(split[k], string, i);
		i = offset;
		k++;
	}
	split[k] = NULL;
	return (1);
}

char	**ft_split(char *string, char sep)
{
	char	**split;
	int	count;

	if (!string)
		return (NULL);
	count = count_words(string, sep);
	split = malloc(sizeof(char *) * (count + 1));
	if (!split)
		return (NULL);
	if (!create_tab(split, string, sep))
		return (NULL);
	return (split);
}
