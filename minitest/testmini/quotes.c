#include "mini.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	count_words(char *string, char sep)
{
	int	i;
	int	in_word;
	int	count;

	i = 0;
	count = 0;
	in_word = 0;
	while(string[i])
	{
		if (string[i] != sep && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (string[i] == sep)
			in_word = 0;
		i++;
	}
	return (count);
}

int	len_word(char *string, char sep, int i)
{
	int	len;
	
	len = 0;

	while (string[i] && string[i] != sep)
	{
		len++;
		i++;
	}
	return (len);
}


void	create_tab(char **split, char *string, char sep)
{
	int	i;
	int	j;
	int	k;
	int	word_len;

	i = 0;
	k = 0;
	while (string[i])
	{
		while (string[i] == sep)
			i++;
		if (string[i] == '\0')
			break;
		word_len = len_word(string, sep, i);
		split[k] = malloc(sizeof(char) * (word_len + 1));
		if (!split[k])
			return;
		j = 0;
		while (j < word_len)
			split[k][j++] = string[i++];
		split[k][j] = '\0';
		k++;
	}
	split[k] = NULL;
}

char	**ft_split(char *string, char sep)
{
	int	count;
	char	**split;

	count = count_words(string, sep);
	split = malloc(sizeof(char *) * (count + 1));
	if (!split)
		return (NULL);
	create_tab(split, string, sep);
	return(split);
}
#include <stdio.h>
int	main()
{
	int	i;
	char	**split;

	i = 0;
	split = ft_split("salut je vais bien", ' ');
	while (split[i])
	{
		printf("%s\n", split[i]);
		i++;
	}
	return (0);
}