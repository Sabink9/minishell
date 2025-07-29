#include "mini.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

int	ft_strcmp(char *s1, char *s2)
{
	int	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	ft_echo(char **split)
{
	int		i = 1;
	int		no_newline = 0;

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
	int	i = 0;

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

int	unclosed_quote(const char *line)
{
	int		i = 0;
	char	quote = 0;

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
		sprintf(joined, "%s%s", line, tmp);
		free(line);
		free(tmp);
		line = joined;
	}
	return (line);
}
void	handle_command(char **split)
{
	int	i = 0;

	if (!split || !split[0])
		return ;
	// while (split[i])
	// {
	// 	printf("[split %d] = \"%s\"\n", i, split[i]);
	// 	i++;
	// }
	if (ft_strcmp(split[0], "echo") == 0)
		ft_echo(split);
	else if (ft_strcmp(split[0], "pwd") == 0)
		ft_pwd();
	else
		printf("minishell: command not found: %s\n", split[0]);
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
		split_line = ft_split(line);
		handle_command(split_line);
		free_split(split_line);
		free(line);
	}
	return (0);
}


