/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:33 by sab               #+#    #+#             */
/*   Updated: 2025/11/12 20:04:06 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

char	get_open_quote(const char *s)
{
	int		i;
	char	q;

	i = 0;
	q = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			if (q == 0)
				q = s[i];
			else if (q == s[i])
				q = 0;
		}
		i++;
	}
	return (q);
}

static void	update_quote_and_append(char **res, char c, char *q)
{
	if (*q == 0)
		*q = c;
	else if (*q == c)
		*q = 0;
	*res = strjoin_char_free(*res, c);
}

char	*process_quotes(char *line)
{
	char	*res;
	int		i;
	char	q;

	res = malloc(1);
	if (!res)
		return (NULL);
	res[0] = '\0';
	i = 0;
	q = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			update_quote_and_append(&res, line[i], &q);
			i++;
			continue ;
		}
		if (q == '\'' && line[i] == '$')
			res = strjoin_char_free(res, (char)-1);
		else
			res = strjoin_char_free(res, line[i]);
		i++;
	}
	return (res);
}
