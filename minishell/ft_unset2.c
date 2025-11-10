/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 12:44:09 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/10 14:35:22 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

/* --- builtin ------------------------------------------------------------ */
/* usage:  int st = ft_unset(argv, &envp);  (st=0 ok,
		st=1 si identifiant invalide) */
int	ft_unset(char **args, char ***penvp)
{
	int	i;
	int	status;

	if (!args || !args[0] || !penvp || !*penvp)
		return (0);
	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			print_unset_error(args[i]);
			status = 1;
		}
		else
			*penvp = env_remove_key(*penvp, args[i]);
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
