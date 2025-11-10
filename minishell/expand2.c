/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 12:29:33 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/10 12:54:51 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

/* Gère le cas $? */
static char	*handle_dollar_exit(char *res, char *line, int *i, int last_exit)
{
	if (line[*i + 1] == '?')
		return (expand_exit_status(res, i, last_exit));
	res = strjoin_char_free(res, '$');
	(*i)++;
	return (res);
}

/* Gère $VAR ou '$' tout seul si pas de nom valide */
static char	*handle_dollar_env(char *res, char *line, int *i, char **envp)
{
	if (line[*i + 1] && (ft_isalnum((unsigned char)line[*i + 1]) || line[*i
				+ 1] == '_'))
		return (expand_env_var(res, line, i, envp));
	res = strjoin_char_free(res, '$');
	(*i)++;
	return (res);
}

char	*expand_variables(char *line, char **envp, int last_exit)
{
	char	*res;
	int		i;

	res = malloc(1);
	if (!res)
		return (NULL);
	res[0] = '\0';
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			if (line[i + 1] == '?')
				res = handle_dollar_exit(res, line, &i, last_exit);
			else
				res = handle_dollar_env(res, line, &i, envp);
			continue ;
		}
		res = append_char(res, line[i]);
		i++;
	}
	return (res);
}
