#include "mini.h"

static int	count_pipes(char **split)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (split[i])
	{
		if (strcmp(split[i], "|") == 0)
			count++;
		i++;
	}
	return (count);
}

char	***parse_pipes(char **split, int *n)
{
	int		i;
	int		j;
	int		k;
	char	***cmdv;

	*n = count_pipes(split);
	cmdv = malloc(sizeof(char **) * (*n + 1));
	if (!cmdv)
		return (NULL);
	i = 0;
	j = 0;
	while (j < *n)
	{
		int	start = i;
		while (split[i] && strcmp(split[i], "|") != 0)
			i++;
		cmdv[j] = malloc(sizeof(char *) * (i - start + 1));
		k = 0;
		while (start < i)
			cmdv[j][k++] = strdup(split[start++]);
		cmdv[j][k] = NULL;
		if (split[i])
			i++;
		j++;
	}
	cmdv[j] = NULL;
	return (cmdv);
}

void	free_cmdv(char ***cmdv)
{
	int	i;
	int	j;

	i = 0;
	while (cmdv && cmdv[i])
	{
		j = 0;
		while (cmdv[i][j])
			free(cmdv[i][j++]);
		free(cmdv[i]);
		i++;
	}
	free(cmdv);
}
