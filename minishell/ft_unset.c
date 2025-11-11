/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kberraho <kberraho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:18 by sab               #+#    #+#             */
/*   Updated: 2025/11/11 17:54:29 by kberraho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

/* renvoie 1 si entry "NAME=..." matche exactement key "NAME" */
static int	entry_matches_key(const char *entry, const char *key)
{
	int	i;

	if (!entry || !key)
		return (0);
	i = 0;
	while (entry[i] && key[i] && entry[i] == key[i])
		i++;
	if (key[i] != '\0')
		return (0);
	return (entry[i] == '=');
}

void	print_unset_error(const char *arg)
{
	write(2, "minishell: unset: `", 20);
	if (arg)
		write(2, arg, ft_strlen((char *)arg));
	write(2, "': not a valid identifier\n", 26);
}

/* retire TOUTES les occurrences de key dans envp -> retourne un nouveau envp */
static int	count_kept_entries(char **envp, const char *key)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (envp && envp[i])
	{
		if (!entry_matches_key(envp[i], key))
			count++;
		i++;
	}
	return (count);
}

static void	copy_kept_entries(char **out, char **envp, const char *key)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp && envp[i])
	{
		if (!entry_matches_key(envp[i], key))
		{
			out[j] = ft_strdup(envp[i]);
			j++;
		}
		i++;
	}
	out[j] = NULL;
}

char	**env_remove_key(char **envp, const char *key)
{
	int		keep;
	char	**out;

	if (!key || !envp)
		return (envp);
	keep = count_kept_entries(envp, key);
	out = malloc(sizeof(char *) * (keep + 1));
	if (!out)
	{
		free_split(envp);
		return (NULL);
	}
	copy_kept_entries(out, envp, key);
	free_split(envp);
	return (out);
}
