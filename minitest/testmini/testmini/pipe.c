#include "../libft/libft.h"
#include "mini.h"

static void	child_process(char **args, char **envp, int in_fd, int out_fd)
{
	char	*exec_path;

	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	/* redirections locales à ce segment */
	if (handle_redirections(args))
		exit(1);
	exec_path = find_executable(args[0], envp);
	if (!exec_path)
	{
		if (!ft_strchr(args[0], '/'))
			printf("minishell: command not found: %s\n", args[0]);
		exit(127);
	}
	execve(exec_path, args, envp);
	perror("execve");
	exit(126);
}

static int	exec_child(char **cmd, char **envp, int in_fd, int *pipefd,
		int is_last)
{
	if (!is_last)
	{
		close(pipefd[0]);
		child_process(cmd, envp, in_fd, pipefd[1]);
	}
	else
		child_process(cmd, envp, in_fd, STDOUT_FILENO);
	return (0);
}

static int	handle_parent(int *in_fd, int *pipefd, int i, int n)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (i < n - 1)
	{
		close(pipefd[1]);
		*in_fd = pipefd[0];
	}
	return (0);
}

int	exec_piped_commands(char ***cmdv, int n, char **envp)
{
	int i;
	int pipefd[2];
	int in_fd;
	pid_t pid;
	int status;

	i = 0;
	in_fd = STDIN_FILENO;
	while (i < n)
	{
		if (i < n - 1 && pipe(pipefd) == -1)
			return (perror("pipe"), 1);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
			exec_child(cmdv[i], envp, in_fd, pipefd, i == n - 1);
		handle_parent(&in_fd, pipefd, i, n);
		i++;
	}
	while (wait(&status) > 0)
		;
	return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}
