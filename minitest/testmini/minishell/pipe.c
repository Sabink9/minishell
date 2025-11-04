#include "../libft/libft.h"
#include "mini.h"

/* last: est-ce le dernier segment du pipe
   last_status: dernier code de sortie connu (pour heredoc $?) */
static void	child_exec(char **cmd, char **envp, int in_fd, int *pipefd,
		int last, int last_status)
{
	char	*path;
	char	**argv2;
	int		rc;
	int		code;

	setup_child_signals();
	/* branchements de pipe */
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
	/* redirections locales (avec expansion heredoc) */
	rc = handle_redirections(cmd, envp, last_status);
	if (rc != 0)
	{
		if (rc == -2)
			exit(130); /* Ctrl-C heredoc */
		else
			exit(1); /* erreur redirection (Permission denied, etc.) */
	}
	/* retire les tokens de redir */
	argv2 = compact_argv(cmd);
	if (!argv2 || !argv2[0])
		exit(0);
	/* builtins dans un pipe (pas d'impression de "exit") */
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
		exit(code); /* pas d’output, juste un code */
	}
	else if (!ft_strcmp(argv2[0], "unset"))
	{
		code = ft_unset(argv2, &envp);
		exit(code);
	}
	/* exécutable externe */
	path = find_executable(argv2[0], envp);
	if (!path)
	{
		if (ft_strchr(argv2[0], '/'))
			exit(126); /* chemin fourni mais inexistant/non exécutable */
		write(2, "minishell: command not found: ", 30);
		write(2, argv2[0], ft_strlen(argv2[0]));
		write(2, "\n", 1);
		exit(127);
	}
	execve(path, argv2, envp);
	perror(argv2[0]);
	exit(126);
}

static int	setup_pipe_and_fork(char ***cmdv, int i, int n, char **envp,
		int *in_fd, int last_status, pid_t *out_pid)
{
	int		pipefd[2];
	pid_t	pid;

	if (i < n - 1 && pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		child_exec(cmdv[i], envp, *in_fd, pipefd, i == n - 1, last_status);
	/* parent */
	*out_pid = pid; /* ✅ on remet le pid au parent */
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (i < n - 1)
	{
		close(pipefd[1]);
		*in_fd = pipefd[0];
	}
	return (0);
}

int	exec_piped_commands(char ***cmdv, int n, char **envp, int last_status)
{
	int		i;
	int		in_fd;
	int		sig;
	pid_t	last_pid;
	pid_t	pid;

	int status, status_last;
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
			last_pid = pid; /* ✅ mémoriser le dernier */
		i++;
	}
	/* attendre d'abord le dernier pour son status exact */
	if (last_pid != -1)
		waitpid(last_pid, &status_last, 0);
	/* puis récolter les autres (et ignorer leur status) */
	while (wait(&status) > 0)
		;
	/* messages d’affichage: se baser sur le dernier */
	if (WIFSIGNALED(status_last))
	{
		sig = WTERMSIG(status_last);
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			write(2, "Quit: 3\n", 8);
		/* ⚠️ ne rien imprimer pour SIGPIPE */
	}
	setup_interactive_signals();
	if (WIFEXITED(status_last))
		return (WEXITSTATUS(status_last));
	if (WIFSIGNALED(status_last))
		return (128 + WTERMSIG(status_last));
	return (1);
}
