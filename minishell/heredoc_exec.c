/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:54:57 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/10 18:07:22 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

static int	hd_loop(int wfd, char *clean, char **envp, int *cfg)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, clean) == 0)
		{
			free(line);
			break ;
		}
		if (hd_handle_line(wfd, line, envp, cfg) == -1)
		{
			free(line);
			return (1);
		}
		free(line);
	}
	return (0);
}

/* ---- enfant très court ---- */

static void	hd_child(int wfd, char *clean, char **envp, int *cfg)
{
	int	st;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	st = hd_loop(wfd, clean, envp, cfg);
	close(wfd);
	free(clean);
	_exit(st);
}

/* ---- parent wait ---- */

static int	hd_parent_wait(pid_t pid, int rfd)
{
	int	st;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &st, 0);
	setup_interactive_signals();
	if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
	{
		write(1, "\n", 1);
		close(rfd);
		return (-2);
	}
	if (WIFEXITED(st) && WEXITSTATUS(st) != 0)
	{
		close(rfd);
		return (-1);
	}
	return (rfd);
}

/* ---- fork + enchaînement compact ---- */

static int	hd_fork_and_run(int pfd[2], char *clean, char **envp, int *cfg)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		close(pfd[0]);
		close(pfd[1]);
		free(clean);
		return (-1);
	}
	if (pid == 0)
	{
		close(pfd[0]);
		hd_child(pfd[1], clean, envp, cfg);
	}
	close(pfd[1]);
	return (hd_parent_wait(pid, pfd[0]));
}

/* ---- fonction principale (courte) ---- */

int	handle_heredoc(char *delim, char **envp, int last_exit)
{
	int		pfd[2];
	char	*clean;
	int		res;
	int		cfg[2];

	g_sig = 0;
	if (hd_setup_pipe(pfd, delim, &clean, &cfg[1]) == -1)
		return (-1);
	cfg[0] = last_exit;
	res = hd_fork_and_run(pfd, clean, envp, cfg);
	free(clean);
	return (res);
}
