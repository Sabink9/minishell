#include "../libft/libft.h"
#include "mini.h"

/* ---------- heredoc ---------- */

static int	delim_is_quoted(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

static char	*delim_unquote(const char *s)
{
	int		i;
	int		k;
	char	*out;

	i = 0;
	k = 0;
	out = malloc(ft_strlen((char *)s) + 1);
	if (!out)
		return (NULL);
	while (s[i])
	{
		if (s[i] != '\'' && s[i] != '"')
			out[k++] = s[i];
		i++;
	}
	out[k] = '\0';
	return (out);
}

/* avant: int handle_heredoc(char *delim) */
int	handle_heredoc(char *delim, char **envp, int last_exit)
{
	int		pfd[2];
	pid_t	pid;
	int		st;
	int		do_expand;
	char	*clean;
	char	*line;
	char	*exp;

	g_sig = 0; /* repart d’un état propre */
	/* si le délimiteur contient des quotes => pas d’expansion */
	do_expand = !delim_is_quoted(delim);
	clean = delim_unquote(delim); /* enlève les quotes du délimiteur */
	if (!clean)
		return (-1);
	if (pipe(pfd) == -1)
	{
		free(clean);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		close(pfd[0]);
		close(pfd[1]);
		free(clean);
		return (-1);
	}
	if (pid == 0)
	{
		/* ----- ENFANT : lit le heredoc et écrit dans pfd[1] ----- */
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(pfd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line)
				break ;
			if (ft_strcmp(line, clean) == 0)
			{
				free(line);
				break ;
			}
			if (do_expand)
			{
				exp = expand_variables(line, envp, last_exit);
				if (!exp)
				{
					free(line);
					close(pfd[1]);
					free(clean);
					_exit(1);
				}
				write(pfd[1], exp, ft_strlen(exp));
				write(pfd[1], "\n", 1);
				free(exp);
			}
			else
			{
				write(pfd[1], line, ft_strlen(line));
				write(pfd[1], "\n", 1);
			}
			free(line);
		}
		close(pfd[1]);
		free(clean);
		_exit(0);
	}
	/* ----- PARENT : récupère le FD de lecture ou gère Ctrl-C ----- */
	close(pfd[1]);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &st, 0);
	setup_interactive_signals();
	free(clean);
	if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
	{
		write(1, "\n", 1); /* retour visuel comme bash */
		close(pfd[0]);
		return (-2); /* l’appelant traduira en 130 */
	}
	if (WIFEXITED(st) && WEXITSTATUS(st) != 0)
	{
		close(pfd[0]);
		return (-1); /* erreur interne (ex: malloc) */
	}
	return (pfd[0]); /* FD prêt à dup2(STDIN_FILENO) */
}

static char	*unquote_token(const char *s)
{
	int		i;
	char	q;
	char	*out;
	int		k;
	int		c;

	i = 0;
	k = 0;
	q = 0;
	out = malloc(ft_strlen((char *)s) + 1);
	if (!out)
		return (NULL);
	while (s[i])
	{
		c = s[i];
		if (c == '\'' || c == '\"')
		{
			if (!q)
				q = c;
			else if (q == c)
				q = 0;
			/* on NE copie PAS la quote */
		}
		else
		{
			if (c == (char)-1)
				c = '$'; /* remet $ visible */
			out[k++] = c;
		}
		i++;
	}
	out[k] = '\0';
	return (out);
}

char	**compact_argv(char **args)
{
	int		i;
	char	**out;
	int		keep;
	int		k;

	i = 0, keep = 0, k = 0;
	while (args && args[i])
	{
		if (args[i] != NULL)
			keep++;
		i++;
	}
	out = malloc(sizeof(char *) * (keep + 1));
	if (!out)
		return (NULL);
	i = 0;
	while (args && args[i])
	{
		if (args[i] != NULL)
			out[k++] = unquote_token(args[i]); /* ⬅️ on enlève les quotes ICI */
		i++;
	}
	out[k] = NULL;
	return (out);
}

/* ---------- main handler ---------- */

/* avant: int handle_redirections(char **args) */
int	handle_redirections(char **args, char **envp, int last_exit)
{
	int	i;
	int	hd;

	i = 0;
	while (args && args[i])
	{
		if (!ft_strcmp(args[i], "<") || !ft_strcmp(args[i], ">")
			|| !ft_strcmp(args[i], ">>") || !ft_strcmp(args[i], "<<"))
		{
			if (!args[i + 1])
			{
				const char *msg =
					"minishell: syntax error near unexpected token `newline'\n";
				write(2, msg, ft_strlen((char *)msg));
				return (-1);
			}
			if (!ft_strcmp(args[i], "<"))
			{
				if (redir_in(args[i + 1]))
					return (-1);
			}
			else if (!ft_strcmp(args[i], ">"))
			{
				if (redir_out(args[i + 1], 0))
					return (-1);
			}
			else if (!ft_strcmp(args[i], ">>"))
			{
				if (redir_out(args[i + 1], 1))
					return (-1);
			}
			else
			{ /* "<<": heredoc */
				hd = handle_heredoc(args[i + 1], envp, last_exit);
				if (hd == -1)
					return (-1);
				if (hd == -2)
					return (-2);
				if (dup2(hd, STDIN_FILENO) < 0)
				{
					perror("dup2");
					close(hd);
					return (-1);
				}
				close(hd);
			}
			args[i] = NULL;
			args[i + 1] = NULL;
			i += 2;
		}
		else
		{
			i++;
		}
	}
	return (0);
}

/* Restaure les stdin/stdout d’origine après exécution */
int	restore_std_fds(int saved_in, int saved_out)
{
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (0);
}
