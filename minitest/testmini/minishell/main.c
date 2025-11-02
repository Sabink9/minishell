#include "../libft/libft.h"
#include "mini.h"

char	**handle_command(char **envp, char **split, int *exit_status)
{
	char	***cmdv;
	int		cmd_count;
	int		saved_in;
	int		saved_out;
	char	**argv;
	int		rc;

	if (!split || !split[0])
		return (envp);
	/* --- PIPE --- */
	if (has_pipe(split))
	{
		cmdv = parse_pipes(split, &cmd_count);
		if (!cmdv)
			return (envp);
		*exit_status = exec_piped_commands(cmdv, cmd_count, envp, *exit_status);
		free_cmdv(cmdv);
		return (envp);
	}
	/* --- REDIRS + BUILTINS/EXEC (commande simple) --- */
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in < 0 || saved_out < 0)
		return (envp);
	rc = handle_redirections(split, envp, *exit_status);
	if (rc != 0)
	{
		*exit_status = (rc == -2) ? 130 : 1; /* ^C HDOC -> 130, sinon 1 */
		restore_std_fds(saved_in, saved_out);
		return (envp);
	}
	argv = compact_argv(split);
	if (!argv)
	{
		restore_std_fds(saved_in, saved_out);
		return (envp);
	}
	if (!argv[0])
	{ /* ex: juste “> a.txt” */
		free_split(argv);
		restore_std_fds(saved_in, saved_out);
		return (envp);
	}
	if (!ft_strcmp(argv[0], "echo"))
		*exit_status = ft_echo(argv);
	else if (!ft_strcmp(argv[0], "pwd"))
		*exit_status = ft_pwd();
	else if (!ft_strcmp(argv[0], "export"))
		(envp = ft_export(envp, argv), *exit_status = 0);
	else if (!ft_strcmp(argv[0], "cd"))
		*exit_status = ft_cd(argv, envp);
	else if (!ft_strcmp(argv[0], "env"))
		*exit_status = ft_env(argv, envp);
	else if (!ft_strcmp(argv[0], "exit"))
		ft_exit(argv, exit_status, 0); /* parent only */
	else
		*exit_status = exec_command(argv, envp);
	free_split(argv);
	restore_std_fds(saved_in, saved_out);
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
			free (line);
			continue ;
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
