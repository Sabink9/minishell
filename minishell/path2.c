/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:46:34 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/12 20:02:17 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

int	search_slash(char **args, char **out, int *need_free)
{
	if (access(args[0], F_OK) != 0)
	{
		perror(args[0]);
		return (127);
	}
	if (access(args[0], X_OK) != 0)
	{
		perror(args[0]);
		return (126);
	}
	*out = args[0];
	*need_free = 0;
	return (0);
}

static int	resolve_path(char **args, char **envp, char **out, int *need_free)
{
	if (ft_strchr(args[0], '/'))
		return (search_slash(args, out, need_free));
	*out = find_executable(args[0], envp);
	if (!*out)
	{
		write(2, "minishell: command not found: ", 30);
		write(2, args[0], ft_strlen(args[0]));
		write(2, "\n", 1);
		return (127);
	}
	*need_free = 1;
	return (0);
}

static void	child_exec(char *path, char **args, char **envp)
{
	setup_child_signals();
	execve(path, args, envp);
	perror(args[0]);
	exit(126);
}

static int	wait_status_to_code(pid_t pid)
{
	int	status;
	int	sig;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			write(2, "Quit: (core dumped)\n", 20);
		return (128 + sig);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	exec_command(char **args, char **envp)
{
	char	*path;
	int		need_free;
	int		rc;
	pid_t	pid;

	rc = resolve_path(args, envp, &path, &need_free);
	if (rc != 0)
		return (rc);
	parent_ignore_signals();
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		setup_interactive_signals();
		if (need_free)
			free(path);
		return (1);
	}
	if (pid == 0)
		child_exec(path, args, envp);
	if (need_free)
		free(path);
	rc = wait_status_to_code(pid);
	setup_interactive_signals();
	return (rc);
}
