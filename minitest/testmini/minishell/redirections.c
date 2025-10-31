#include "../libft/libft.h"
#include "mini.h"

/* ---------- heredoc ---------- */

static int	handle_heredoc(char *delim)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
			break ;
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(pipefd[1]);
	return (pipefd[0]);
}

/* ---------- main handler ---------- */

int	handle_redirections(char **args)
{
	int	i;
	int	heredoc_fd;

	i = 0;
	while (args[i])
	{
		if (!args[i + 1])
			break ;
		if (ft_strcmp(args[i], "<") == 0 && redir_in(args[i + 1]))
			return (-1);
		else if (ft_strcmp(args[i], ">") == 0 && redir_out(args[i + 1], 0))
			return (-1);
		else if (ft_strcmp(args[i], ">>") == 0 && redir_out(args[i + 1], 1))
			return (-1);
		else if (ft_strcmp(args[i], "<<") == 0)
		{
			heredoc_fd = handle_heredoc(args[i + 1]);
			if (heredoc_fd == -1)
				return (-1);
			dup2(heredoc_fd, STDIN_FILENO);
			close(heredoc_fd);
		}
		skip_args(args, &i);
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
