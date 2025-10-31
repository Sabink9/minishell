#include "../libft/libft.h"
#include "mini.h"

char	*get_path_from_env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*join_dir_cmd(const char *dir, int len_dir, const char *cmd)
{
	char	*res;
	int		len_cmd;
	int		i;
	int		j;

	len_cmd = ft_strlen(cmd);
	if (len_dir == 0)
		len_dir = 1;
	res = malloc(len_dir + len_cmd + 2);
	if (!res)
		return (NULL);
	i = 0;
	while (i < len_dir && dir[i])
	{
		res[i] = dir[i];
		i++;
	}
	if (i == 0)
		res[0] = '.';
	res[i++] = '/';
	j = 0;
	while (j < len_cmd)
		res[i++] = cmd[j++];
	res[i] = '\0';
	return (res);
}

static char	*search_in_path(const char *path_env, char *cmd)
{
	const char	*p;
	const char	*seg_start;
	char		*candidate;
	int			seg_len;

	p = path_env;
	seg_start = p;
	while (1)
	{
		if (*p == ':' || *p == '\0')
		{
			seg_len = p - seg_start;
			candidate = join_dir_cmd(seg_start, seg_len, cmd);
			if (!candidate)
				return (NULL);
			if (access(candidate, X_OK) == 0)
				return (candidate);
			free(candidate);
			if (*p == '\0')
				break ;
			seg_start = p + 1;
		}
		p++;
	}
	return (NULL);
}

char	*find_executable(char *cmd, char **envp)
{
	char	*path_env;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) != 0)
		{
			perror(cmd);
			return (NULL);
		}
		if (access(cmd, X_OK) != 0)
		{
			perror(cmd);
			return (NULL);
		}
		return (ft_strdup(cmd));
	}
	path_env = get_path_from_env(envp);
	if (!path_env)
		return (NULL);
	return (search_in_path(path_env, cmd));
}

#include "../libft/libft.h"
#include "mini.h"
#include <signal.h>

int	exec_command(char **args, char **envp)
{
	pid_t	pid;
	int		status;
	char	*exec_path;
	int		sig;

	exec_path = find_executable(args[0], envp);
	if (!exec_path)
	{
		if (!ft_strchr(args[0], '/'))
		{
			write(2, "minishell: command not found: ", 30);
			write(2, args[0], ft_strlen(args[0]));
			write(2, "\n", 1);
			return (127);
		}
		perror(args[0]);
		return (126);
	}
	/* Parent : ignorer les signaux pendant l'exécution */
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		setup_interactive_signals(); /* restaurer même en erreur */
		free(exec_path);
		return (1);
	}
	if (pid == 0)
	{
		setup_child_signals(); /* enfant -> comportement par défaut */
		execve(exec_path, args, envp);
		perror(args[0]);
		exit(126);
	}
	free(exec_path);
	waitpid(pid, &status, 0);
	/* Parent : retour en mode interactif (readline) */
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)       /* Ctrl-C */
			write(1, "\n", 1);   /* évite ^C$> en collé */
		else if (sig == SIGQUIT) /* Ctrl-\ */
			write(2, "Quit: 3\n", 8);
	}
	setup_interactive_signals();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
