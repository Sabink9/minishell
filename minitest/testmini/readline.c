#include "mini.h"

int ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void ft_echo(char **split)
{
	int i;
	int no_newline;

	i = 1;
	no_newline = 0;
	if (!split[0] || ft_strcmp(split[0], "echo") != 0)
	{
		printf("Format : \"echo <string>\"\n");
		return;
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

void free_split(char **tab)
{
	int i;

	i = 0;
	if (!tab)
		return;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
}

int unclosed_quote(const char *line)
{
	int i;
	char quote;

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

#include "mini.h"

char *ft_strjoin(char *s1, char *s2)
{
	char *res;
	int i;
	int j;

	if (!s1 || !s2)
		return (NULL);
	res = malloc(strlen(s1) + strlen(s2) + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		res[i + j] = s2[j];
		j++;
	}
	res[i + j] = '\0';
	return (res);
}

char *read_full_line(void)
{
	char *line;
	char *tmp;
	char *joined;

	line = readline("$> ");
	while (line && unclosed_quote(line))
	{
		tmp = readline("> ");
		if (!tmp)
			break;
		joined = ft_strjoin(line, tmp);
		free(line);
		free(tmp);
		if (!joined)
			return (NULL);
		line = joined;
	}
	return (line);
}

t_env *handle_command(t_env *env, char **split)
{
	// int	i = 0;
	if (!split || !split[0])
		return (env);
	// while (split[i])
	// {
	// 	printf("[split %d] = \"%s\"\n", i, split[i]);
	// 	i++;
	// }
	if (ft_strcmp(split[0], "echo") == 0)
		ft_echo(split);
	else if (ft_strcmp(split[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(split[0], "export") == 0)
		env = ft_export(env, split);
	else
		printf("minishell: command not found: %s\n", split[0]);
	return (env);
}


int main(void)
{
	char *line;
	char **split_line;
	t_env *env;

	env = NULL;
	while (1)
	{
		line = read_full_line();
		if (!line)
			break;
		split_line = ft_split(line);
		if (split_line && split_line[0])
		{
			if (ft_strcmp(split_line[0], "exit") == 0)
			{
				free_split(split_line);
				free_env_list(env);
				printf("exit\n");
				exit(0);
			}
			else
				env = handle_command(env, split_line);
		}
		free_split(split_line);
		free(line);
	}
	free_env_list(env);
	return (0);
}
