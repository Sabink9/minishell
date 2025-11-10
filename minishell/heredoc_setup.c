/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 17:57:06 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/10 18:06:23 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

/* ---------- heredoc ---------- */

void	remove_n_tokens(char **args, int pos, int n)
{
	int	j;

	j = pos;
	while (args[j + n])
	{
		args[j] = args[j + n];
		j++;
	}
	args[j] = NULL;
}

static int	delim_is_quoted(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

static char	*delim_unquote(const char *s)
{
	int		i;
	int		k;
	char	*out;

	i = 0;
	k = 0;
	out = malloc(ft_strlen((char *)s) + 1);
	if (!out)
		return (NULL);
	while (s[i])
	{
		if (s[i] != '\'' && s[i] != '"')
			out[k++] = s[i];
		i++;
	}
	out[k] = '\0';
	return (out);
}

/* avant: int handle_heredoc(char *delim) */
int	hd_setup_pipe(int pfd[2], char *delim, char **clean, int *do_expand)
{
	*do_expand = !delim_is_quoted(delim);
	*clean = delim_unquote(delim);
	if (!*clean)
		return (-1);
	if (pipe(pfd) == -1)
	{
		free(*clean);
		return (-1);
	}
	return (0);
}

int	hd_handle_line(int wfd, char *line, char **envp, int *cfg)
{
	char	*exp;

	if (cfg[1])
	{
		exp = expand_variables(line, envp, cfg[0]);
		if (!exp)
			return (-1);
		write(wfd, exp, ft_strlen(exp));
		write(wfd, "\n", 1);
		free(exp);
	}
	else
	{
		write(wfd, line, ft_strlen(line));
		write(wfd, "\n", 1);
	}
	return (0);
}
