/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sab <sab@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:23 by sab               #+#    #+#             */
/*   Updated: 2025/11/06 20:00:24 by sab              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		char **new_env;

	if (!split || !split[0])
		return (envp);
	if (has_pipe(split))
	{
		cmdv = parse_pipes(split, &cmd_count);
		if (!cmdv)
			return (envp);
		*exit_status = exec_piped_commands(cmdv, cmd_count, envp, *exit_status);
		free_cmdv(cmdv);
		return (envp);
	}
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in < 0 || saved_out < 0)
		return (envp);
	rc = handle_redirections(split, envp, *exit_status);
	if (rc != 0)
	{
		if (rc == -2)
			*exit_status = 130;
		else
			*exit_status = 1;
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
	{
		free_split(argv);
		restore_std_fds(saved_in, saved_out);
		return (envp);
	}
	if (!ft_strcmp(argv[0], "echo"))
		*exit_status = ft_echo(argv);
	else if (!ft_strcmp(argv[0], "pwd"))
		*exit_status = ft_pwd();
	else if (!ft_strcmp(argv[0], "export"))
	{
		new_env = ft_export(envp, argv, exit_status);
		if (new_env && new_env != envp) 
			envp = new_env;
	}
	else if (!ft_strcmp(argv[0], "cd"))
	{
		if (argv[1] && argv[2])
		{
			write(2, "minishell: cd: too many arguments\n", 34);
			*exit_status = 1;
		}
		else
			*exit_status = ft_cd(argv, envp);
	}
	else if (!ft_strcmp(argv[0], "env"))
		*exit_status = ft_env(argv, envp);
	else if (!ft_strcmp(argv[0], "exit"))
		ft_exit(argv, exit_status, 0);
	else if (!ft_strcmp(argv[0], "unset"))
	{
		new_env = NULL;
		*exit_status = ft_unset(argv, &new_env);
		if (new_env)
			envp = new_env;
	}
	else
		*exit_status = exec_command(argv, envp);
	free_split(argv);
	restore_std_fds(saved_in, saved_out);
	return (envp);
}

int	main(int argc, char **argv, char **envp_sys)
{
	int		exit_status;
	char	*line;
	char	**split_line;
	char	**envp;

	(void)argc;
	(void)argv;
	envp = ft_env_dup(envp_sys);
	if (!envp)
		return (1);
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
			free(line);
			continue ;
		}
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
			envp = handle_command(envp, split_line, &exit_status);
		free_split(split_line);
		free(line);
	}
	rl_clear_history();
	free_split(envp);
	return (exit_status);
}
