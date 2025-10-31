#include "../libft/libft.h"
#include "mini.h"

char	**handle_command(char **envp, char **split, int *exit_status)
{
	char	***cmdv;
	int		cmd_count;

	if (!split || !split[0])
		return (envp);
	if (has_pipe(split))
	{
		cmdv = parse_pipes(split, &cmd_count);
		if (!cmdv)
			return (envp);
		*exit_status = exec_piped_commands(cmdv, cmd_count, envp);
		free_cmdv(cmdv);
		return (envp);
	}
	if (ft_strcmp(split[0], "echo") == 0)
		*exit_status = ft_echo(split);
	else if (ft_strcmp(split[0], "pwd") == 0)
		*exit_status = ft_pwd();
	else if (ft_strcmp(split[0], "export") == 0)
	{
		envp = ft_export(envp, split);
		*exit_status = 0;
	}
	else if (ft_strcmp(split[0], "cd") == 0)
		*exit_status = ft_cd(split, envp);
	else if (ft_strcmp(split[0], "env") == 0)
		*exit_status = ft_env(split, envp);
	else if (ft_strcmp(split[0], "exit") == 0)
		ft_exit(split, exit_status, 0);
	else
		*exit_status = exec_command(split, envp);
	return (envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		exit_status;
	char	*line;
	char	**split_line;

	(void)argc;
	(void)argv;
	exit_status = 0;
	setup_interactive_signals();
	while (1)
	{
		line = read_full_line();
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (g_sig == SIGINT)
		{
			exit_status = 130;
			g_sig = 0;
		}
		// 🔹 Si la ligne est vide, on repart directement
		if (*line == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		line = process_quotes(line);
		line = expand_variables(line, envp, exit_status);
		split_line = ft_split(line);
		if (split_line && split_line[0])
		{
			if (split_line && split_line[0])
				envp = handle_command(envp, split_line, &exit_status);
		}
		free_split(split_line);
		free(line);
	}
	rl_clear_history();
	return (exit_status);
}
