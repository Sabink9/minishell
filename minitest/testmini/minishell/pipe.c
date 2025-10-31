#include "../libft/libft.h"
#include "mini.h"

static void	child_exec(char **cmd, char **envp, int in_fd, int *pipefd, int last)
{
	char	*path;
	int		code = 0;

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

	// 🔹 Gérer les builtins localement dans le pipe
	if (ft_strcmp(cmd[0], "exit") == 0)
	{
		ft_exit(cmd, &code, 1);
		exit(code); // quitte le process enfant uniquement
	}
	else if (ft_strcmp(cmd[0], "echo") == 0)
		exit(ft_echo(cmd));
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		exit(ft_pwd());
	else if (ft_strcmp(cmd[0], "env") == 0)
		exit(ft_env(cmd, envp));

	path = find_executable(cmd[0], envp);
	if (!path)
	{
		write(2, "minishell: command not found: ", 30);
		write(2, cmd[0], ft_strlen(cmd[0]));
		write(2, "\n", 1);
		exit(127);
	}
	execve(path, cmd, envp);
	perror(cmd[0]);
	exit(126);
}

static int	setup_pipe_and_fork(char ***cmdv, int i, int n, char **envp,
		int *in_fd)
{
	int		pipefd[2];
	pid_t	pid;

	if (i < n - 1 && pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		child_exec(cmdv[i], envp, *in_fd, pipefd, i == n - 1);
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
	int	i;
	int	in_fd;
	int	status;
	int	sig;

	/* Parent: ignorer les signaux pendant l’exécution du pipeline */
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	i = 0;
	in_fd = STDIN_FILENO;
	while (i < n)
	{
		if (setup_pipe_and_fork(cmdv, i, n, envp, &in_fd))
		{
			/* Restaure les signaux interactifs avant de sortir sur erreur */
			setup_interactive_signals();
			return (1);
		}
		i++;
	}
	while (wait(&status) > 0)
		;
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			write(2, "Quit: 3\n", 8);
	}
	/* Parent: restauration du mode interactif (readline/handlers) */
	setup_interactive_signals();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
