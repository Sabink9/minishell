/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:31 by sab               #+#    #+#             */
/*   Updated: 2025/11/10 16:53:36 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

void	child_setup_io(int in_fd, int *pipefd, int is_last)
{
	if (!is_last)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
}

static char	**prepare_argv(char **cmd, char **envp, int last_status)
{
	char	**argv2;
	int		rc;

	rc = handle_redirections(cmd, envp, last_status);
	if (rc != 0)
	{
		if (rc == -2)
			exit(130);
		exit(1);
	}
	argv2 = compact_argv(cmd);
	if (!argv2 || !argv2[0])
		exit(0);
	return (argv2);
}

static int	builtin_dispatch(char **argv2, char **envp)
{
	int	code;

	if (!ft_strcmp(argv2[0], "exit"))
	{
		code = 0;
		ft_exit(argv2, &code, 1);
		return (code);
	}
	if (!ft_strcmp(argv2[0], "echo"))
		return (ft_echo(argv2));
	if (!ft_strcmp(argv2[0], "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(argv2[0], "env"))
		return (ft_env(argv2, envp));
	if (!ft_strcmp(argv2[0], "export"))
	{
		code = 0;
		(void)ft_export(envp, argv2, &code);
		return (code);
	}
	if (!ft_strcmp(argv2[0], "unset"))
		return (ft_unset(argv2, &envp));
	return (-1);
}

static void	exec_external(char **argv2, char **envp)
{
	char	*path;

	path = find_executable(argv2[0], envp);
	if (!path)
	{
		if (ft_strchr(argv2[0], '/'))
			exit(126);
		write(2, "minishell: command not found: ", 30);
		write(2, argv2[0], ft_strlen(argv2[0]));
		write(2, "\n", 1);
		exit(127);
	}
	execve(path, argv2, envp);
	perror(argv2[0]);
	exit(126);
}

void	child_process(char **cmd, char **envp, int last_status)
{
	char	**argv2;
	int		code;

	setup_child_signals();
	argv2 = prepare_argv(cmd, envp, last_status);
	code = builtin_dispatch(argv2, envp);
	if (code >= 0)
		exit(code);
	exec_external(argv2, envp);
}
