/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:23 by sab               #+#    #+#             */
/*   Updated: 2025/11/10 18:48:46 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

/* restaure et gère l’erreur de redirection */
static char	**redir_error(int rc, int *exit_status, int *fd, char **envp)
{
	if (rc == -2)
		*exit_status = 130;
	else
		*exit_status = 1;
	restore_std_fds(fd[0], fd[1]);
	return (envp);
}

/* exécute une commande simple sans pipes */
static char	**handle_simple_command(char **split, char **envp, int *exit_status)
{
	int		rc;
	int		fd[2];
	char	**argv;
	char	**new_env;

	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);
	if (fd[0] < 0 || fd[1] < 0)
		return (envp);
	rc = handle_redirections(split, envp, *exit_status);
	if (rc != 0)
		return (redir_error(rc, exit_status, fd, envp));
	argv = compact_argv(split);
	if (!argv || !argv[0])
	{
		if (argv)
			free_split(argv);
		restore_std_fds(fd[0], fd[1]);
		return (envp);
	}
	new_env = exec_builtin_or_cmd(argv, envp, exit_status);
	free_split(argv);
	restore_std_fds(fd[0], fd[1]);
	return (new_env);
}

/* fonction principale (< 25 lignes, 3 arguments, 3 appels) */
char	**handle_command(char **envp, char **split, int *exit_status)
{
	if (!split || !split[0])
		return (envp);
	if (has_pipe(split))
		return (handle_pipes(split, envp, exit_status));
	return (handle_simple_command(split, envp, exit_status));
}

/* fonction principale du shell */
int	main(int argc, char **argv, char **envp_sys)
{
	char	*line;
	char	**envp;
	int		exit_status;

	(void)argc;
	(void)argv;
	envp = init_env(envp_sys);
	exit_status = 0;
	while (1)
	{
		line = read_full_line();
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (handle_empty_or_signal(line, &exit_status))
			continue ;
		envp = process_line(line, envp, &exit_status);
	}
	rl_clear_history();
	free_split(envp);
	return (exit_status);
}
