#include "mini.h"

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

char	**ft_split(char *string, char sep)
{
	int	i;
	int	count;
	char	**split;

	count = count_words(string, sep);
	split = malloc(sizeof(char *) * (count + 1));
	if (!split)
		return (NULL);
}