#include "mini.h"
#include "../libft/libft.h"

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

