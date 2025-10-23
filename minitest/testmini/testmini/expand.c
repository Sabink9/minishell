#include "mini.h"

/* ajoute un caractère à une chaîne et free l’ancienne */
static char	*strjoin_char_free(char *s, char c)
{
	int		len;
	char	*res;
	int		i;

	len = 0;
	while (s && s[len])
		len++;
	res = malloc(len + 2);
	if (!res)
		return (free(s), NULL);
	i = -1;
	while (++i < len)
		res[i] = s[i];
	res[i++] = c;
	res[i] = '\0';
	free(s);
	return (res);
}

/* concatène deux chaînes et free la première */
static char	*strjoin_free(char *s1, char *s2)
{
	int		len1;
	int		len2;
	char	*res;
	int		i;
	int		j;

	len1 = 0;
	while (s1 && s1[len1])
		len1++;
	len2 = 0;
	while (s2 && s2[len2])
		len2++;
	res = malloc(len1 + len2 + 1);
	if (!res)
		return (free(s1), NULL);
	i = -1;
	while (++i < len1)
		res[i] = s1[i];
	j = 0;
	while (j < len2)
		res[i++] = s2[j++];
	res[i] = '\0';
	free(s1);
	return (res);
}

/* récupère la valeur d’une variable env */
static char	*get_env_value(char *name, char **envp)
{
	int	i;
	int	len;

	len = 0;
	while (name[len])
		len++;
	i = 0;
	while (envp && envp[i])
	{
		if (!strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return ("");
}

/* expansion principale */
char	*expand_variables(char *line, char **envp, int last_exit)
{
	char	*res;
	int		i;
	char	buf[12];
	int		n;
	int		j;
	char	name[256];
	int		k;

	res = malloc(1);
	if (!res)
		return (NULL);
	res[0] = '\0';
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i] != -1 && line[i + 1])
		{
			if (line[i + 1] == '?')
			{
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
				i += 2;
				continue ;
			}
			j = i + 1;
			while (line[j] && ((line[j] >= 'a' && line[j] <= 'z')
					|| (line[j] >= 'A' && line[j] <= 'Z') || (line[j] >= '0'
						&& line[j] <= '9') || line[j] == '_'))
				j++;
			k = 0;
			while (i + 1 + k < j && k < 255)
			{
				name[k] = line[i + 1 + k];
				k++;
			}
			name[k] = '\0';
			res = strjoin_free(res, get_env_value(name, envp));
			i = j;
			continue ;
		}
		if (line[i] == -1)
			res = strjoin_char_free(res, '$');
		else
			res = strjoin_char_free(res, line[i]);
		i++;
	}
	return (res);
}
