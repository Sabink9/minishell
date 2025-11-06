/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sab <sab@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:31 by sab               #+#    #+#             */
/*   Updated: 2025/11/06 20:16:13 by sab              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

/* last: est-ce le dernier segment du pipe
   last_status: dernier code de sortie connu (pour heredoc $?) */
static void child_exec(char **cmd, char **envp, int in_fd, int *pipefd,
		       int last, int last_status)
{
	char *path;
	char **argv2;
	int rc;
	int code;

	setup_child_signals();
	if (!last)
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
	rc = handle_redirections(cmd, envp, last_status);
	if (rc != 0)
	{
		if (rc == -2)
			exit(130);
		else
			exit(1);
	}
	argv2 = compact_argv(cmd);
	if (!argv2 || !argv2[0])
		exit(0);
	if (!ft_strcmp(argv2[0], "exit"))
	{
		code = 0;
		ft_exit(argv2, &code, 1);
		exit(code);
	}
	else if (!ft_strcmp(argv2[0], "echo"))
	{
		exit(ft_echo(argv2));
	}
	else if (!ft_strcmp(argv2[0], "pwd"))
	{
		exit(ft_pwd());
	}
	else if (!ft_strcmp(argv2[0], "env"))
	{
		exit(ft_env(argv2, envp));
	}
	else if (!ft_strcmp(argv2[0], "export"))
	{
		code = 0;
		(void)ft_export(envp, argv2, &code);
		exit(code);
	}
	else if (!ft_strcmp(argv2[0], "unset"))
	{
		code = ft_unset(argv2, &envp);
		exit(code);
	}
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

static int setup_pipe_and_fork(char ***cmdv, int i, int n, char **envp,
			       int *in_fd, int last_status, pid_t *out_pid)
{
	int pipefd[2];
	pid_t pid;

	if (i < n - 1 && pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		child_exec(cmdv[i], envp, *in_fd, pipefd, i == n - 1, last_status);
	*out_pid = pid;
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (i < n - 1)
	{
		close(pipefd[1]);
		*in_fd = pipefd[0];
	}
	return (0);
}

int exec_piped_commands(char ***cmdv, int n, char **envp, int last_status)
{
	int i;
	int in_fd;
	int sig;
	pid_t last_pid;
	pid_t pid;
	int status;
	int status_last;

	last_pid = -1;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	i = 0;
	in_fd = STDIN_FILENO;
	while (i < n)
	{
		if (setup_pipe_and_fork(cmdv, i, n, envp, &in_fd, last_status, &pid))
		{
			setup_interactive_signals();
			return (1);
		}
		if (i == n - 1)
			last_pid = pid;
		i++;
	}
	if (last_pid != -1)
		waitpid(last_pid, &status_last, 0);
	while (wait(&status) > 0)
	{
	}
	if (WIFSIGNALED(status_last))
	{
		sig = WTERMSIG(status_last);
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			write(2, "Quit: 3\n", 8);
		else if (sig == SIGPIPE)
			perror("Broken pipe");
	}
	setup_interactive_signals();
	if (WIFEXITED(status_last))
		return (WEXITSTATUS(status_last));
	if (WIFSIGNALED(status_last))
		return (128 + WTERMSIG(status_last));
	return (1);
}
