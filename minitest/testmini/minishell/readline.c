#include "../libft/libft.h"
#include "mini.h"

int	ft_echo(char **split)
{
	int	i;
	int	no_newline;

	i = 1;
	no_newline = 0;
	if (!split[0] || ft_strcmp(split[0], "echo") != 0)
		return (1);
	if (split[1] && ft_strcmp(split[1], "-n") == 0)
	{
		no_newline = 1;
		i = 2;
	}
	while (split[i])
	{
		printf("%s", split[i]);
		if (split[i + 1])
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
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
	/* ⬇️ IMPORTANT : si Ctrl-C a frappé pendant le prompt */
	if (g_sig == SIGINT)
	{
		g_sig = 0;
		free(line);
		return (ft_strdup("")); /* ligne vide => main continuera au prompt */
	}
	while (unclosed_quote(line))
	{
		tmp = readline("> ");
		if (!tmp)
		{ /* Ctrl-D en continuation => on annule */
			free(line);
			return (ft_strdup(""));
		}
		/* ⬇️ Ctrl-C pendant la continuation */
		if (g_sig == SIGINT)
		{
			g_sig = 0;
			free(tmp);
			free(line);
			return (ft_strdup(""));
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
