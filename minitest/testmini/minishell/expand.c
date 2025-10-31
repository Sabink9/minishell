#include "mini.h"
#include "../libft/libft.h"

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
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return ("");
}

/* expansion principale */
static char	*expand_exit_status(char *res, int *i, int last_exit)
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

static char	*expand_env_var(char *res, char *line, int *i, char **envp)
{
	char	name[256];
	int		j;
	int		k;

	j = *i + 1;
	while (line[j] && ((line[j] >= 'a' && line[j] <= 'z')
			|| (line[j] >= 'A' && line[j] <= 'Z')
			|| (line[j] >= '0' && line[j] <= '9') || line[j] == '_'))
		j++;
	k = 0;
	while (*i + 1 + k < j && k < 255)
	{
		name[k] = line[*i + 1 + k];
		k++;
	}
	name[k] = '\0';
	res = strjoin_free(res, get_env_value(name, envp));
	*i = j;
	return (res);
}

static char	*append_char(char *res, char c)
{
	if (c == -1)
		return (strjoin_char_free(res, '$'));
	return (strjoin_char_free(res, c));
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
		if (line[i] == '$' && line[i + 1])
		{
			if (line[i + 1] == '?')
				res = expand_exit_status(res, &i, last_exit);
			else
				res = expand_env_var(res, line, &i, envp);
			continue ;
		}
		res = append_char(res, line[i]);
		i++;
	}
	return (res);
}
