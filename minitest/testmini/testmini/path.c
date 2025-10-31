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

void	exec_command(char **args, char **envp)
{
	pid_t pid;
	char *exec_path = find_executable(args[0], envp);

	if (!exec_path)
	{
		if (!ft_strchr(args[0], '/'))
		{
			write(2, "minishell: command not found: ", 31);
			write(2, args[0], ft_strlen(args[0]));
			write(2, "\n", 1);
		}

		return ;
	}

	pid = fork();
	if (pid == 0)
		execve(exec_path, args, envp);
	else
		waitpid(pid, NULL, 0);

	free(exec_path);
}
