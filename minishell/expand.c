/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 19:59:50 by sab               #+#    #+#             */
/*   Updated: 2025/11/11 20:15:33 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

/* récupère la valeur d’une variable env */
char	*get_env_value(const char *key, char **envp)
{
	int		i;
	size_t	len;

	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

/* expansion principale */
char	*expand_exit_status(char *res, int *i, int last_exit)
{
	char	buf[12];
	int		n;
	int		j;

	n = last_exit;
	j = 11;
	buf[j--] = '\0';
	if (n == 0)
		buf[j--] = '0';
	while (n > 0)
	{
		buf[j--] = (n % 10) + '0';
		n /= 10;
	}
	res = strjoin_free(res, buf + j + 1);
	*i += 2;
	return (res);
}

static char	*extract_var_name(char *line, int *i)
{
	int		j;
	int		k;
	char	*name;
	int		line_len;

	line_len = ft_strlen(line);
	name = malloc(line_len + 1);
	if (!name)
		return (NULL);
	j = *i + 1;
	k = 0;
	while (line[j] && (ft_isalnum((unsigned char)line[j]) || line[j] == '_'))
	{
		name[k++] = line[j];
		j++;
	}
	name[k] = '\0';
	*i = j;
	return (name);
}

char	*expand_env_var(char *res, char *line, int *i, char **envp)
{
	char	*name;
	char	*val;

	name = extract_var_name(line, i);
	if (!name)
		return (res);
	val = get_env_value(name, envp);
	if (val)
		res = strjoin_free(res, val);
	else
		res = strjoin_free(res, "");
	free(name);
	return (res);
}

char	*append_char(char *res, char c)
{
	if (c == -1)
		return (strjoin_char_free(res, '$'));
	return (strjoin_char_free(res, c));
}
