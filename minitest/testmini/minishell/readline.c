#include "../libft/libft.h"
#include "mini.h"

int	ft_echo(char **split)
{
	int	i;
	int	no_newline;

	i = 1;
	no_newline = 0;
	if (!split[0] || ft_strcmp(split[0], "echo") != 0)
		return (1);
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
	return (0);
}

int	unclosed_quote(const char *line)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
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

char	*read_full_line(void)
{
	char	*line;
	char	*tmp;
	char	*joined;

	line = readline("$> ");
	if (!line)
		return (NULL);
	while (unclosed_quote(line))
	{
		tmp = readline("> ");
		if (!tmp)
			break ;
		joined = ft_strjoin(line, tmp);
		free(line);
		free(tmp);
		if (!joined)
			return (NULL);
		line = joined;
	}
	return (line);
}

int	has_pipe(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		if (ft_strcmp(split[i], "|") == 0)
			return (1);
		i++;
	}
	return (0);
}
