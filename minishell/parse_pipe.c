/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:26 by sab               #+#    #+#             */
/*   Updated: 2025/11/11 15:44:24 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

static int	check_pipe_syntax(char **split)
{
	int	i;

	if (!split || !split[0])
		return (0);
	if (ft_strcmp(split[0], "|") == 0)
		return (1);
	i = 0;
	while (split[i])
	{
		if (ft_strcmp(split[i], "|") == 0)
		{
			if (!split[i + 1] || ft_strcmp(split[i + 1], "|") == 0)
				return (1);
		}
		i++;
	}
	return (0);
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

static char	***init_pipe_array(char **split, int *n)
{
	char	***cmdv;

	if (check_pipe_syntax(split))
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
		g_sig = 2;
		*n = 0;
		return (NULL);
	}
	*n = count_pipes(split);
	cmdv = malloc(sizeof(char **) * (*n + 1));
	if (!cmdv)
		return (NULL);
	return (cmdv);
}

/* découpe les segments de commandes séparés par | */
char	***parse_pipes(char **split, int *n)
{
	int		i;
	int		j;
	int		start;
	char	***cmdv;

	cmdv = init_pipe_array(split, n);
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
