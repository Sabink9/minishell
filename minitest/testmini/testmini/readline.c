#include "mini.h"
#include "../libft/libft.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	ft_echo(char **split)
{
	int	i;
	int	no_newline;

	i = 1;
	no_newline = 0;
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

char	*ft_strjoin(char *s1, char *s2)
{
	char	*res;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	res = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
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

char	*read_full_line(void)
{
	char	*line;
	char	*tmp;
	char	*joined;

	line = readline("$> ");
	while (line && unclosed_quote(line))
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

char	**handle_command(char **envp, char **split)
{
	int		saved_in;
	int		saved_out;
	char	***cmdv;
	int		cmd_count;

	if (!split || !split[0])
		return (envp);
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	/* 🔹 1. Si c’est un pipeline, ne fais AUCUNE redirection globale ici */
	if (has_pipe(split))
	{
		cmdv = parse_pipes(split, &cmd_count);
		if (cmdv)
		{
			exec_piped_commands(cmdv, cmd_count, envp);
			free_cmdv(cmdv);
		}
		restore_std_fds(saved_in, saved_out);
		return (envp);
	}
	/* 🔹 2. Sinon, c’est une commande simple → on gère les redirections ici */
	if (handle_redirections(split) == -1)
	{
		restore_std_fds(saved_in, saved_out);
		return (envp);
	}
	if (ft_strcmp(split[0], "echo") == 0)
		ft_echo(split);
	else if (ft_strcmp(split[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(split[0], "export") == 0)
		envp = ft_export(envp, split);
	else if (ft_strcmp(split[0], "cd") == 0)
		envp = ft_cd(split, envp);
	else if (ft_strcmp(split[0], "exit") == 0)
	{
		printf("exit\n");
		exit(0);
	}
	else
		exec_command(split, envp);
	restore_std_fds(saved_in, saved_out);
	return (envp);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**split_line;

	// t_env	*env;
	(void)argc;
	(void)argv;
	while (1)
	{
		line = read_full_line();
		if (!line)
			break ;
		if (*line)
			add_history(line);
		line = process_quotes(line);
		line = expand_variables(line, envp, 0); // ou last_exit si tu le gères
		split_line = ft_split(line);
		if (split_line && split_line[0])
		{
			if (ft_strcmp(split_line[0], "exit") == 0)
			{
				free_split(split_line);
				printf("exit\n");
				rl_clear_history();
				exit(0);
			}
			else
				envp = handle_command(envp, split_line);
		}
		free_split(split_line);
		free(line);
	}
	rl_clear_history();
	return (0);
}
