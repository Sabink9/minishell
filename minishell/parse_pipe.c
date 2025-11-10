/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:26 by sab               #+#    #+#             */
/*   Updated: 2025/11/10 18:42:07 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini.h"
#include "../libft/libft.h"

static int	count_pipes(char **split)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (split[i])
	{
		if (ft_strcmp(split[i], "|") == 0)
			count++;
		i++;
	}
	return (count);
}

static char	**copy_pipe_segment(char **split, int start, int end)
{
	char	**cmd;
	int		k;

	cmd = malloc(sizeof(char *) * (end - start + 1));
	if (!cmd)
		return (NULL);
	k = 0;
	while (start < end)
	{
		cmd[k] = ft_strdup(split[start]);
		if (!cmd[k])
			return (NULL);
		k++;
		start++;
	}
	cmd[k] = NULL;
	return (cmd);
}

char	***parse_pipes(char **split, int *n)
{
	int		i;
	int		j;
	int		start;
	char	***cmdv;

	*n = count_pipes(split);
	cmdv = malloc(sizeof(char **) * (*n + 1));
	if (!cmdv)
		return (NULL);
	i = 0;
	j = 0;
	while (j < *n)
	{
		start = i;
		while (split[i] && ft_strcmp(split[i], "|") != 0)
			i++;
		cmdv[j] = copy_pipe_segment(split, start, i);
		if (split[i])
			i++;
		j++;
	}
	cmdv[j] = NULL;
	return (cmdv);
}

int	finalize_status(int status_last)
{
	int	sig;

	if (WIFSIGNALED(status_last))
	{
		sig = WTERMSIG(status_last);
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			write(2, "Quit: 3\n", 8);
		else if (sig == SIGPIPE)
			perror("Broken pipe");
		return (128 + sig);
	}
	if (WIFEXITED(status_last))
		return (WEXITSTATUS(status_last));
	return (1);
}
