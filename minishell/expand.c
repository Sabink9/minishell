#include "../libft/libft.h"
#include "mini.h"

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
	char	*val;

	j = *i + 1;
	k = 0;
	/* Lire le nom de la variable */
	while (line[j] && ((line[j] >= 'a' && line[j] <= 'z') || (line[j] >= 'A'
				&& line[j] <= 'Z') || (line[j] >= '0' && line[j] <= '9')
			|| line[j] == '_'))
	{
		name[k++] = line[j];
		j++;
	}
	name[k] = '\0';
	val = get_env_value(name, envp);
	/* Ajouter la valeur */
	res = strjoin_free(res, val);
	*i = j;
	return (res);
}

static char	*append_char(char *res, char c)
{
	if (c == -1)
		return (strjoin_char_free(res, '$'));
	return (strjoin_char_free(res, c));
}

/* Gère l'expansion après un '$' dans la ligne */
static char	*handle_dollar(char *res, char *line, int *i, char **envp,
		int last_exit)
{
	/* $?: expansion du code de retour */
	if (line[*i + 1] == '?')
		return (expand_exit_status(res, i, last_exit));
	/* $NAME: expansion d'une variable d'environnement */
	if (line[*i + 1] && (ft_isalnum((unsigned char)line[*i + 1]) || line[*i
			+ 1] == '_'))
		return (expand_env_var(res, line, i, envp));
	/* '$' suivi d'un caractère non valide → conserver '$' littéral */
	res = strjoin_char_free(res, '$');
	(*i)++;
	return (res);
}

/* Parcourt la ligne et remplace les variables $VAR et $? */
char	*expand_variables(char *line, char **envp, int last_exit)
{
	char	*res;
	int		i;

	/* résultat initial vide */
	res = malloc(1);
	if (!res)
		return (NULL);
	res[0] = '\0';
	i = 0;
	while (line[i])
	{
		/* lorsqu'on rencontre '$', traitement spécial */
		if (line[i] == '$')
		{
			res = handle_dollar(res, line, &i, envp, last_exit);
			continue ;
		}
		/* sinon, copier le caractère dans le résultat */
		res = append_char(res, line[i]);
		i++;
	}
	return (res);
}
