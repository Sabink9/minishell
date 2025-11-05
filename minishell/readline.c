#include "../libft/libft.h"
#include "mini.h"

int	ft_echo(char **argv)
{
	int	i;
	int	no_newline;
	int	first;
	int	len;

	if (!argv || !argv[0] || ft_strcmp(argv[0], "echo") != 0)
		return (1);
	i = 1;
	no_newline = 0;
	if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
	{
		no_newline = 1;
		i = 2;
	}
	first = 1;
	while (argv[i])
	{
		len = ft_strlen(argv[i]);
		if (!first)
			write(1, " ", 1);
		if (len > 0)
			write(1, argv[i], len);
		first = 0;
		i++;
	}
	if (!no_newline)
		write(1, "\n", 1);
	return (0);
}

static int	unclosed_quote(const char *s)
{
	int		i;
	char	q;

	i = 0;
	q = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			if (!q)
				q = s[i];
			else if (q == s[i])
				q = 0;
		}
		i++;
	}
	return (q != 0);
}

char	*read_full_line(void)
{
	char	*line;
	char	*tmp;
	char	*joined;

	line = readline("$> ");
	if (!line) /* Ctrl-D au prompt => quitter proprement */
		return (NULL);
	/* >>> NE PLUS TOUCHER A g_sig ICI <<< */
	while (unclosed_quote(line))
	{
		tmp = readline("> ");
		if (!tmp)
		{ /* Ctrl-D pendant continuation => annuler */
			free(line);
			return (ft_strdup(""));
			/* ligne vide => main fera juste un prompt */
		}
		if (g_sig == SIGINT)
		{
			/* Ctrl-C pendant la continuation : on annule la saisie */
			free(tmp);
			free(line);
			return (ft_strdup(""));
			/* ligne vide => main verra g_sig et mettra 130 */
		}
		joined = ft_strjoin(line, tmp);
		free(line);
		free(tmp);
		if (!joined)
			return (NULL);
		line = joined;
	}
	return (line);
}

int	has_pipe(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		if (ft_strcmp(split[i], "|") == 0)
			return (1);
		i++;
	}
	return (0);
}
