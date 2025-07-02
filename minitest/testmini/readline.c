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

char	*unescape_all(char *str)
{
	int		i = 0;
	int		j = 0;
	char	*res;

	res = malloc(strlen(str) + 1);
	if (!res)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
			i++; // on saute le backslash
		res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}

void	ft_echo(char **split)
{
	int		i = 1;
	int		no_newline = 0;
	char	*tmp;

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
		tmp = unescape_all(split[i]);
		if (!tmp)
			return ; // gérer malloc fail
		printf("%s", tmp);
		free(tmp);
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

int	is_escapedpuff(const char *str, int pos)
{
	int	count = 0;
	pos--;
	while (pos >= 0 && str[pos] == '\\')
	{
		count++;
		pos--;
	}
	return (count % 2 == 1);
}


int	unclosed_quote(const char *line)
{
	int		i = 0;
	char	quote = 0;

	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '"') && !is_escapedpuff(line, i))
		{
			if (!quote)
				quote = line[i];
			else if (quote == line[i])
				quote = 0;
		}
		i++;
	}
	return (quote != 0);
}


int	line_ends_with_unescaped_backslash(const char *line)
{
	int	len = 0;

	if (!line)
		return (0);
	while (line[len])
		len++;
	if (len == 0)
		return (0);
	return (line[len - 1] == '\\' && !is_escapedpuff(line, len - 1));
}


char	*read_full_line(void)
{
	char	*line = readline("$> ");
	char	*tmp;
	char	*joined;

	while (line && (unclosed_quote(line) || line_ends_with_unescaped_backslash(line)))
	{
		tmp = readline("> ");
		if (!tmp)
			break ;
		// Si la ligne se termine par \ non échappé, on supprime ce \ avant concat
		if (line_ends_with_unescaped_backslash(line))
		{
			line[strlen(line) - 1] = '\0'; // supprime le \ final
		}
		joined = malloc(strlen(line) + strlen(tmp) + 2);
		if (!joined)
			return (free(line), free(tmp), NULL);
		sprintf(joined, "%s%s", line, tmp); // concat sans \n car \ final supprime
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
