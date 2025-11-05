#include "../libft/libft.h"
#include "mini.h"

static void	print_args(char **argv, int i)
{
	int	first;
	int	len;

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
}

int	ft_echo(char **argv)
{
	int	i;
	int	no_newline;

	if (!argv || !argv[0] || ft_strcmp(argv[0], "echo") != 0)
		return (1);
	i = 1;
	no_newline = 0;
	if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
	{
		no_newline = 1;
		i = 2;
	}
	print_args(argv, i);
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

static char	*continue_line(char *line)
{
	char	*tmp;
	char	*joined;

	while (unclosed_quote(line))
	{
		tmp = readline("> ");
		if (!tmp)
		{
			free(line);
			return (ft_strdup(""));
		}
		if (g_sig == SIGINT)
		{
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

char	*read_full_line(void)
{
	char	*line;

	line = readline("$> ");
	if (!line)
		return (NULL);
	line = continue_line(line);
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
