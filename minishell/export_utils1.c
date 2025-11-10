/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:59:55 by sab               #+#    #+#             */
/*   Updated: 2025/11/10 12:42:55 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

int	envp_size(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

int	find_env_index(char **envp, const char *key)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(key);
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void	print_export(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		printf("declare -x %s\n", envp[i]);
		i++;
	}
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

char	*make_env_var(const char *key, const char *value)
{
	int		i;
	int		j;
	char	*new_var;
	int		len_key;
	int		len_value;

	len_key = ft_strlen(key);
	len_value = 0;
	if (value)
		len_value = ft_strlen(value);
	new_var = malloc(len_key + len_value + 2);
	if (!new_var)
		return (NULL);
	i = 0;
	while (key[i])
	{
		new_var[i] = key[i];
		i++;
	}
	new_var[i++] = '=';
	j = 0;
	while (value && value[j])
		new_var[i++] = value[j++];
	new_var[i] = '\0';
	return (new_var);
}
