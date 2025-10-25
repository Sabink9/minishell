#include "mini.h"
#include "../libft/libft.h"

static int	envp_size(char **envp)
{
	int	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

static int	find_env_index(char **envp, const char *key)
{
	int	i = 0;
	int	len = ft_strlen(key);

	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static void	print_export(char **envp)
{
	int	i = 0;
	while (envp && envp[i])
	{
		printf("declare -x %s\n", envp[i]);
		i++;
	}
}

static char	*make_env_var(const char *key, const char *value)
{
	int		len_key = ft_strlen(key);
	int		len_value = value ? ft_strlen(value) : 0;
	char	*new_var;
	int		i = 0;
	int		j = 0;

	new_var = malloc(len_key + len_value + 2); // '=' + '\0'
	if (!new_var)
		return (NULL);
	while (key[i])
	{
		new_var[i] = key[i];
		i++;
	}
	new_var[i++] = '=';
	if (value)
	{
		while (value[j])
			new_var[i++] = value[j++];
	}
	new_var[i] = '\0';
	return (new_var);
}

static char	**set_env_var(char **envp, const char *key, const char *value)
{
	int		idx = find_env_index(envp, key);
	int		size = envp_size(envp);
	char	*new_var;
	char	**new_envp;
	int		i;

	new_var = make_env_var(key, value);
	if (!new_var)
		return (envp);

	if (idx >= 0)
	{
		free(envp[idx]);
		envp[idx] = new_var;
		return (envp);
	}

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

static void	split_key_value(const char *str, char **key, char **value)
{
	int	i = 0;

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
	int		i = 1;
	char	*key;
	char	*value;

	if (!args[1])
	{
		print_export(envp);
		return (envp);
	}
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