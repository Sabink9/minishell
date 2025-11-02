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

	/* Si le nom contient '/', on laisse exec_command gérer les accès/erreurs */
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	/* Sinon: chercher dans PATH */
	path_env = get_path_from_env(envp);
	if (!path_env)
		return (NULL);
	return (search_in_path(path_env, cmd)); /* NULL si introuvable */
}

/* ---------- exec_command.c ---------- */
int	exec_command(char **args, char **envp)
{
	pid_t	pid;
	int		status;
	char	*path;
	int		need_free;
	int		sig;

	need_free = 0;
	/* Cas 1: chemin avec '/' -> traiter comme pathname direct */
	if (ft_strchr(args[0], '/'))
	{
		/* Noter: ici path n'est pas alloué, on utilise args[0] directement */
		if (access(args[0], F_OK) != 0)
		{
			perror(args[0]);
			return (127);
		} /* n'existe pas */
		if (access(args[0], X_OK) != 0)
		{
			perror(args[0]);
			return (126);
		} /* pas exécutable */
		path = args[0];
	}
	else
	{
		/* Cas 2: recherche dans PATH */
		path = find_executable(args[0], envp);
		if (!path)
		{
			write(2, "minishell: command not found: ", 30);
			write(2, args[0], ft_strlen(args[0]));
			write(2, "\n", 1);
			return (127);
		}
		need_free = 1;
			/* path vient de malloc (ft_strdup dans search_in_path) */
	}
	/* Parent: ignorer Ctrl-C / Ctrl-\ pendant l'exécution */
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		/* Revenir en mode interactif quoi qu'il arrive */
		setup_interactive_signals();
		if (need_free)
			free(path);
		return (1);
	}
	if (pid == 0)
	{
		/* Enfant: comportements par défaut des signaux */
		setup_child_signals();
		execve(path, args, envp);
		/* Ici: trouvé mais impossible à exécuter (ex: format invalide ENOEXEC) */
		perror(args[0]);
		_exit(126);
	}
	/* Parent */
	if (need_free)
		free(path);
	waitpid(pid, &status, 0);
	/* Rétablir le mode readline (SIGINT/SIGQUIT custom) */
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)       /* Ctrl-C */
			write(1, "\n", 1);   /* évite "^C$>" collé */
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
