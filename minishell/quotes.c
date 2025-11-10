/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:33 by sab               #+#    #+#             */
/*   Updated: 2025/11/10 12:58:27 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

/* pré-traite la ligne : supprime les quotes et marque les zones protégées */
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
