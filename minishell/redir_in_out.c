/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_in_out.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:38 by sab               #+#    #+#             */
/*   Updated: 2025/11/13 11:21:38 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

int	redir_in(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	redir_out(const char *file, int append)
{
	int	fd;

	if (append)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(file);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

// pour redirec.c
void	preprocess_loop(char *line, char *out)
{
	int	i;
	int	k;
	int	in_s;
	int	in_d;

	i = 0;
	k = 0;
	in_s = 0;
	in_d = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !in_d)
			in_s = !in_s;
		if (line[i] == '"' && !in_s)
			in_d = !in_d;
		if (!in_s && !in_d && (line[i] == '<' || line[i] == '>'))
			add_redir_char(out, line, &i, &k);
		else
			out[k++] = line[i];
		i++;
	}
	out[k] = '\0';
}
