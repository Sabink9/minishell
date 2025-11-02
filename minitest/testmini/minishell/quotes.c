#include "../libft/libft.h"
#include "mini.h"

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
			/* ⬇️ on CONSERVE les quotes */
			res = strjoin_char_free(res, line[i]);
			i++;
			continue ;
		}
		if (q == '\'' && line[i] == '$')
			res = strjoin_char_free(res, (char)-1); /* bloque expansion */
		else
			res = strjoin_char_free(res, line[i]);
		i++;
	}
	return (res);
}
