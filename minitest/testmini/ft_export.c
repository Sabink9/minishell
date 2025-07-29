#include "mini.h"

static int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

static void	sort_env(t_env **arr, int size)
{
	int		i;
	int		swapped;
	t_env	*tmp;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < size - 1)
		{
			if (ft_strcmp(arr[i]->key, arr[i + 1]->key) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
}

static int	env_list_size(t_env *head)
{
	int		size = 0;
	t_env	*tmp = head;

	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	return (size);
}

static t_env	**env_list_to_array(t_env *head, int size)
{
	t_env	**arr;
	int		i;
	t_env	*tmp;

	arr = malloc(sizeof(t_env *) * size);
	if (!arr)
		return (NULL);
	i = 0;
	tmp = head;
	while (tmp)
	{
		arr[i++] = tmp;
		tmp = tmp->next;
	}
	return (arr);
}

static void	print_export(t_env *head)
{
	int		size;
	t_env	**arr;
	int		i;

	size = env_list_size(head);
	arr = env_list_to_array(head, size);
	if (!arr)
		return ;
	sort_env(arr, size);
	i = 0;
	while (i < size)
	{
		printf("declare -x %s", arr[i]->key);
		if (arr[i]->value)
			printf("=\"%s\"", arr[i]->value);
		printf("\n");
		i++;
	}
	free(arr);
}

static t_env	*find_env_node(t_env *head, const char *key)
{
	t_env	*tmp = head;

	while (tmp)
	{
		if (strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

static int	set_env_var(t_env **head, const char *key, const char *value)
{
	t_env	*node;

	node = find_env_node(*head, key);
	if (node)
	{
		free(node->value);
		node->value = value ? strdup(value) : NULL;
		if (value && !node->value)
			return (-1);
		return (0);
	}
	node = malloc(sizeof(t_env));
	if (!node)
		return (-1);
	node->key = strdup(key);
	node->value = value ? strdup(value) : NULL;
	if ((!node->key) || (value && !node->value))
	{
		free(node->key);
		free(node->value);
		free(node);
		return (-1);
	}
	node->next = *head;
	*head = node;
	return (0);
}

void	free_env_list(t_env *head)
{
	t_env	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->key);
		free(head->value);
		free(head);
		head = tmp;
	}
}

static void	split_key_value(const char *str, char **key, char **value)
{
	int	i = 0;

	while (str[i] && str[i] != '=')
		i++;
	*key = strndup(str, i);
	if (str[i] == '=')
		*value = strdup(str + i + 1);
	else
		*value = NULL;
}

t_env	*ft_export(t_env *env, char **args)
{
	int		i = 1;
	char	*key;
	char	*value;

	if (!args[1])
	{
		print_export(env);
		return (env);
	}
	while (args[i])
	{
		split_key_value(args[i], &key, &value);
		if (set_env_var(&env, key, value) == -1)
		{
			printf("minishell: export: allocation error\n");
			free(key);
			free(value);
			break ;
		}
		free(key);
		free(value);
		i++;
	}
	return (env);
}
