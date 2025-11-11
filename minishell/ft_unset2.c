/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 12:44:09 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/11 15:27:47 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

static int	unset_one(char *arg, char ***penvp)
{
	if (!is_valid_identifier(arg))
	{
		print_unset_error(arg);
		return (1);
	}
	*penvp = env_remove_key(*penvp, arg);
	if (!*penvp)
		return (1);
	return (0);
}

int	ft_unset(char **args, char ***penvp)
{
	int	i;
	int	status;

	if (!args || !args[1] || !penvp || !*penvp)
		return (0);
	status = 0;
	i = 1;
	while (args[i])
	{
		if (unset_one(args[i], penvp))
			status = 1;
		i++;
	}
	return (status);
}

static int	count_env_entries(char **envp)
{
	int	count;

	count = 0;
	while (envp && envp[count])
		count++;
	return (count);
}

char	**ft_env_dup(char **envp)
{
	char	**out;
	int		count;
	int		i;

	count = count_env_entries(envp);
	out = (char **)malloc(sizeof(char *) * (count + 1));
	if (!out)
		return (NULL);
	i = 0;
	while (i < count)
	{
		out[i] = ft_strdup(envp[i]);
		if (!out[i])
			return (free_dup_until(out, i), NULL);
		i++;
	}
	out[count] = NULL;
	return (out);
}
