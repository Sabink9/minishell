/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:29 by sab               #+#    #+#             */
/*   Updated: 2025/11/10 14:58:48 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

static char	*get_path_from_env(char **envp)
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
		return (ft_strdup(cmd));
	path_env = get_path_from_env(envp);
	if (!path_env)
		return (NULL);
	return (search_in_path(path_env, cmd));
}

void	parent_ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
