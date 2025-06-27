#include "mini.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
	{
		i++;
	}
	return (s1[i] - s2[i]);
}

void	ft_echo(char **split)
{
	int	i = 1;
	int	no_newline = 0;

	if (!split[0] || ft_strcmp(split[0], "echo") != 0)
	{
		printf("Format : \"echo <string>\"\n");
		return ;
	}

	if (split[1] && ft_strcmp(split[1], "-n") == 0)
	{
		no_newline = 1;
		i = 2;
	}

	while (split[i])
	{
		printf("%s", split[i]);
		if (split[i + 1])
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
}

void	free_split(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL; // Ajout de sécurité
		i++;
	}
	free(tab);
}


int	unclosed_quote(const char *line)
{
	int		i = 0;
	char	quote = 0;

	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '"'))
		{
			// Si on n'est pas dans une quote, on en ouvre une
			if (!quote)
				quote = line[i];
			// Si on est dans une quote et qu'on trouve la même → on la ferme
			else if (quote == line[i])
				quote = 0;
		}
		i++;
	}
	return (quote != 0);
}


char	*read_full_line(void)
{
	char	*line = readline("$> ");
	char	*tmp;
	char	*joined;

	while (line && unclosed_quote(line))
	{
		tmp = readline("> ");
		if (!tmp)
			break ;
		joined = malloc(strlen(line) + strlen(tmp) + 2);
		if (!joined)
			return (free(line), free(tmp), NULL);
		sprintf(joined, "%s\n%s", line, tmp);
		free(line);
		free(tmp);
		line = joined;
	}
	return (line);
}

int	main(void)
{
	char	*line;
	char	**split_line;

	while (1)
	{
		line = read_full_line();
		if (!line)
			break ;
		split_line = ft_split(line, ' ');
		ft_echo(split_line);
		free_split(split_line);
		free(line);
	}
	return (0);
}
