/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:30:07 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/11 20:15:44 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

int	is_n_flag(char *arg)
{
	int	i;

	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

int	ft_echo(char **argv)
{
	int	i;
	int	no_newline;

	i = 1;
	no_newline = 0;
	while (argv[i] && is_n_flag(argv[i]))
	{
		no_newline = 1;
		i++;
	}
	while (argv[i])
	{
		write(1, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!no_newline)
		write(1, "\n", 1);
	return (0);
}

//pour pipe
void	child_setup_io(int in_fd, int *pipefd, int is_last)
{
	if (!is_last)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
}

// parse_pipe
int	count_pipes(char **split)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (split[i])
	{
		if (ft_strcmp(split[i], "|") == 0)
			count++;
		i++;
	}
	return (count);
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
