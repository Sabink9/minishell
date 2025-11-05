#include "../libft/libft.h"
#include "mini.h"

static char	*get_env_value(char **envp, const char *key)
{
	int		i;
	size_t	len;

	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

static void	set_env_value(char **envp, const char *key, const char *value)
{
	int		i;
	size_t	len;
	char	*new_entry;

	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
		{
			new_entry = malloc(ft_strlen(key) + ft_strlen(value) + 2);
			if (!new_entry)
				return ;
			ft_strcpy(new_entry, key);
			ft_strcat(new_entry, "=");
			ft_strcat(new_entry, value);
			envp[i] = new_entry;
			return ;
		}
		i++;
	}
}

static int	change_directory(char **args, char **envp, char *oldpwd,
		char *newpwd)
{
	char	*target;

	if (!args[1])
		target = get_env_value(envp, "HOME");
	else
		target = args[1];
	if (!target)
	{
		write(2, "minishell: cd: HOME not set\n", 29);
		return (1);
	}
	if (chdir(target) != 0)
	{
		perror("cd");
		return (1);
	}
	if (!getcwd(newpwd, 1024))
	{
		perror("cd");
		return (1);
	}
	set_env_value(envp, "OLDPWD", oldpwd);
	set_env_value(envp, "PWD", newpwd);
	return (0);
}

int	ft_cd(char **args, char **envp)
{
	char	oldpwd[1024];
	char	newpwd[1024];

	if (!getcwd(oldpwd, 1024))
	{
		perror("cd");
		return (1);
	}
	return (change_directory(args, envp, oldpwd, newpwd));
}
