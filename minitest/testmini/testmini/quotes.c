#include "mini.h"
#include "../libft/libft.h"

/* ajoute un caractère à une chaîne et libère l’ancienne */
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

/* pré-traite la ligne : supprime les quotes et marque les zones protégées */
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
		if ((line[i] == '\'' || line[i] == '\"'))
		{
			if (q == 0)
				q = line[i];
			else if (q == line[i])
				q = 0;
			i++;
			continue ;
		}
		if (q == '\'')
		{
			if (line[i] == '$')
				line[i] = -1; /* bloque expansion dans quotes simples */
		}
		res = strjoin_char_free(res, line[i]);
		i++;
	}
	return (res);
}

