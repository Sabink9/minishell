#include "mini.h"

int handle_redirections(char **args)
{
	int i = 0;
	int fd;

	while (args[i])
	{
		/* Sortie écrasée : ">" */
		if (strcmp(args[i], ">") == 0 && args[i + 1])
		{
			fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror(args[i + 1]);
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			args[i] = NULL;
			args[i + 1] = NULL;
			i += 2;
			continue;
		}
		/* Sortie append : ">>" */
		else if (strcmp(args[i], ">>") == 0 && args[i + 1])
		{
			fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				perror(args[i + 1]);
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			args[i] = NULL;
			args[i + 1] = NULL;
			i += 2;
			continue;
		}
		/* Entrée : "<" */
		else if (strcmp(args[i], "<") == 0 && args[i + 1])
		{
			fd = open(args[i + 1], O_RDONLY);
			if (fd < 0)
			{
				perror(args[i + 1]);
				return (-1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			args[i] = NULL;
			args[i + 1] = NULL;
			i += 2;
			continue;
		}
		i++;
	}
	return (0);
}

/* Restaure les stdin/stdout d’origine après exécution */
int restore_std_fds(int saved_in, int saved_out)
{
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (0);
}
