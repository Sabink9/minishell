/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 15:55:06 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/10 18:27:37 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

static pid_t	launch_stage_mid(char **cmd, char **envp, int *in_fd, int st)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), (pid_t) - 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), (pid_t) - 1);
	if (pid == 0)
	{
		child_setup_io(*in_fd, pipefd, 0);
		child_process(cmd, envp, st);
	}
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	close(pipefd[1]);
	*in_fd = pipefd[0];
	return (pid);
}

static pid_t	launch_stage_last(char **cmd, char **envp, int *in_fd, int st)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), (pid_t) - 1);
	if (pid == 0)
	{
		child_setup_io(*in_fd, NULL, 1);
		child_process(cmd, envp, st);
	}
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	return (pid);
}

static pid_t	run_pipeline_loop(char ***cmdv, int n, char **envp, int st)
{
	int		i;
	int		in_fd;
	pid_t	pid;
	pid_t	last_pid;

	i = 0;
	in_fd = STDIN_FILENO;
	last_pid = (pid_t) - 1;
	while (i < n)
	{
		if (i < n - 1)
			pid = launch_stage_mid(cmdv[i], envp, &in_fd, st);
		else
			pid = launch_stage_last(cmdv[i], envp, &in_fd, st);
		if (pid == (pid_t) - 1)
			return ((pid_t) - 1);
		if (i == n - 1)
			last_pid = pid;
		i++;
	}
	return (last_pid);
}

static int	wait_all_return_last(pid_t last_pid)
{
	int	status_last;
	int	tmp;

	status_last = 0;
	if (last_pid != -1)
		waitpid(last_pid, &status_last, 0);
	while (wait(&tmp) > 0)
		;
	return (status_last);
}

int	exec_piped_commands(char ***cmdv, int n, char **envp, int last_status)
{
	pid_t	last_pid;
	int		status_last;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	last_pid = run_pipeline_loop(cmdv, n, envp, last_status);
	if (last_pid == (pid_t)-1)
	{
		setup_interactive_signals();
		return (1);
	}
	status_last = wait_all_return_last(last_pid);
	setup_interactive_signals();
	return (finalize_status(status_last));
}
