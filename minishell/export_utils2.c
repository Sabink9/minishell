/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:59:58 by sab               #+#    #+#             */
/*   Updated: 2025/11/10 12:42:46 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

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

static int	handle_export_arg(char ***envp, char *arg)
{
	char	*key;
	char	*value;

	if (!is_valid_identifier(arg))
	{
		write(2, "minishell: export: `", 20);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		return (1);
	}
	split_key_value(arg, &key, &value);
	*envp = set_env_var(*envp, key, value);
	free(key);
	free(value);
	return (0);
}

static int	process_export_args(char ***envp, char **args)
{
	int	i;
	int	had_error;

	i = 1;
	had_error = 0;
	while (args[i])
	{
		if (handle_export_arg(envp, args[i]))
			had_error = 1;
		i++;
	}
	return (had_error);
}

char	**ft_export(char **envp, char **args, int *exit_status)
{
	int	had_error;

	if (!args[1])
	{
		print_export(envp);
		if (exit_status)
			*exit_status = 0;
		return (envp);
	}
	had_error = process_export_args(&envp, args);
	if (exit_status)
		*exit_status = had_error;
	return (envp);
}
