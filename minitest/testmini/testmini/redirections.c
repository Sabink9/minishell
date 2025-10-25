#include "../libft/libft.h"
#include "mini.h"

/* ---------- utils ---------- */

static void	skip_args(char **args, int *i)
{
	args[*i] = NULL;
	args[*i + 1] = NULL;
	*i += 2;
}

static void	redir_in(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror(file));
	dup2(fd, STDIN_FILENO);
	close(fd);
}

static void	redir_out(const char *file, int append)
{
	int	fd;

	if (append)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(file));
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

/* ---------- heredoc ---------- */

static int	handle_heredoc(char *delim)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
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
	return (pipefd[0]); /* on retourne le FD de lecture */
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
		if (ft_strcmp(args[i], "<") == 0)
			redir_in(args[i + 1]);
		else if (ft_strcmp(args[i], ">") == 0)
			redir_out(args[i + 1], 0);
		else if (ft_strcmp(args[i], ">>") == 0)
			redir_out(args[i + 1], 1);
		else if (ft_strcmp(args[i], "<<") == 0)
		{
			heredoc_fd = handle_heredoc(args[i + 1]);
			if (heredoc_fd != -1)
			{
				dup2(heredoc_fd, STDIN_FILENO);
				close(heredoc_fd);
			}
		}
		else
		{
			i++;
			continue ;
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
