/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:36 by sab               #+#    #+#             */
/*   Updated: 2025/11/12 12:30:53 by saciurus         ###   ########.fr       */
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

static char	*get_next_quote_input(char *line, char q)
{
	char	*tmp;

	(void)line;
	tmp = readline("> ");
	if (!tmp)
	{
		printf("minishell: unexpected EOF while looking for matching `%c'\n",
			q);
		printf("minishell: syntax error: unexpected end of file\n");
		g_sig = 2;
		return (NULL);
	}
	if (g_sig == SIGINT)
	{
		g_sig = 0;
		free(tmp);
		return (ft_strdup(""));
	}
	return (tmp);
}

/* assemble la ligne courante + "\n" + la nouvelle saisie */
static char	*read_next_quote_line(char *line, char q)
{
	char	*tmp;
	char	*with_nl;
	char	*joined;

	tmp = get_next_quote_input(line, q);
	if (!tmp)
	{
		free(line);
		return (NULL);
	}
	with_nl = ft_strjoin(line, "\n");
	free(line);
	if (!with_nl)
	{
		free(tmp);
		return (NULL);
	}
	joined = ft_strjoin(with_nl, tmp);
	free(with_nl);
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
