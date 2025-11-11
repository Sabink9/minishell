/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:14:14 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/11 15:30:05 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

int	apply_heredoc(char **args, int i, char **envp, int last_exit)
{
	int	hd;

	if (!args[i + 1])
	{
		write(2, "minishell: syntax error near unexpected token `newline'\n",
			57);
		return (-1);
	}
	hd = handle_heredoc(args[i + 1], envp, last_exit);
	if (hd == -1 || hd == -2)
		return (hd);
	if (dup2(hd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(hd);
		return (-1);
	}
	close(hd);
	remove_n_tokens(args, i, 2);
	return (1);
}

int	finish_file_redir(char *fname, int is_in, int append)
{
	int	r;

	if (is_in)
		r = redir_in(fname);
	else
		r = redir_out(fname, append);
	free(fname);
	if (r != 0)
		return (-1);
	return (0);
}
