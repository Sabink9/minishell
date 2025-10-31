#include "mini.h"
#include "../libft/libft.h"

char	**append_env_var(char **envp, char *new_var, int size)
{
	char	**new_envp;
	int		i;

	new_envp = malloc(sizeof(char *) * (size + 2));
	if (!new_envp)
	{
		free(new_var);
		return (envp);
	}
	i = 0;
	while (i < size)
	{
		new_envp[i] = envp[i];
		i++;
	}
	new_envp[i++] = new_var;
	new_envp[i] = NULL;
	return (new_envp);
}

char	**set_env_var(char **envp, const char *key, const char *value)
{
	int		idx;
	int		size;
	char	*new_var;

	idx = find_env_index(envp, key);
	size = envp_size(envp);
	new_var = make_env_var(key, value);
	if (!new_var)
		return (envp);
	if (idx >= 0)
	{
		free(envp[idx]);
		envp[idx] = new_var;
		return (envp);
	}
	return (append_env_var(envp, new_var, size));
}

void	split_key_value(const char *str, char **key, char **value)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	*key = ft_strndup(str, i);
	if (str[i] == '=')
		*value = ft_strdup(str + i + 1);
	else
		*value = NULL;
}

char	**ft_export(char **envp, char **args)
{
	int		i;
	char	*key;
	char	*value;

	if (!args[1])
	{
		print_export(envp);
		return (envp);
	}
	i = 1;
	while (args[i])
	{
		split_key_value(args[i], &key, &value);
		envp = set_env_var(envp, key, value);
		free(key);
		free(value);
		i++;
	}
	return (envp);
}
