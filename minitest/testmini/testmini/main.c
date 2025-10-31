#include "mini.h"
#include "../libft/libft.h"

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
