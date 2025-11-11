/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:36 by sab               #+#    #+#             */
/*   Updated: 2025/11/11 15:32:03 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

static int	unclosed_quote(const char *s)
{
	int		i;
	char	q;

	i = 0;
	q = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			if (!q)
				q = s[i];
			else if (q == s[i])
				q = 0;
		}
		i++;
	}
	return (q != 0);
}

static char	*read_next_quote_line(char *line, char q)
{
	char	*tmp;
	char	*joined;

	tmp = readline("> ");
	if (!tmp)
	{
		printf("minishell: unexpected EOF while looking for matching `%c'\n", q);
		printf("minishell: syntax error: unexpected end of file\n");
		g_sig = 2;
		free(line);
		return (NULL);
	}
	if (g_sig == SIGINT)
	{
		g_sig = 0;
		free(tmp);
		free(line);
		return (ft_strdup(""));
	}
	joined = ft_strjoin(line, tmp);
	free(line);
	free(tmp);
	return (joined);
}

/* Boucle principale : relit tant qu’une quote reste ouverte */
static char	*continue_line(char *line)
{
	char	q;
	char	*joined;

	while (unclosed_quote(line))
	{
		q = unclosed_quote(line);
		joined = read_next_quote_line(line, q);
		if (!joined)
			return (NULL);
		line = joined;
	}
	return (line);
}

char	*read_full_line(void)
{
	char	*line;

	line = readline("$> ");
	if (!line)
		return (NULL);
	line = continue_line(line);
	return (line);
}

int	has_pipe(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		if (ft_strcmp(split[i], "|") == 0)
			return (1);
		i++;
	}
	return (0);
}
