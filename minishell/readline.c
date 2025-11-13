/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:36 by sab               #+#    #+#             */
/*   Updated: 2025/11/13 09:13:18 by saciurus         ###   ########.fr       */
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

char	*read_full_line(void)
{
	char	*line;

	line = readline("$> ");
	if (!line)
		return (NULL);
	if (unclosed_quote(line))
	{
		printf("minishell: unexpected EOF while looking for matching quote\n");
		free(line);
		return (ft_strdup(""));
	}
	return (line);
}
