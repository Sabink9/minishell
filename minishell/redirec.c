/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:14:14 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/13 10:33:39 by saciurus         ###   ########.fr       */
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
	remove_n_tokens(args, i, 2);
	return (hd);
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

static void	add_redir_char(char *out, char *line, int *i, int *k)
{
	if (*k > 0 && out[*k - 1] != ' ')
		out[(*k)++] = ' ';
	if (line[*i] == '<' && line[*i + 1] == '<' && line[*i + 2] == '<')
	{
		out[(*k)++] = '<';
		out[(*k)++] = '<';
		out[(*k)++] = '<';
		*i += 2;
		return ;
	}
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		out[(*k)++] = '<';
		out[(*k)++] = '<';
		(*i)++;
		return ;
	}
	if (line[*i] == '>' && line[*i + 1] == '>')
	{
		out[(*k)++] = '>';
		out[(*k)++] = '>';
		(*i)++;
		return ;
	}
	out[(*k)++] = line[*i];
}

char	*preprocess_redirs(char *line)
{
	char	*out;
	int		i;
	int		k;

	out = malloc(ft_strlen(line) * 2 + 1);
	if (!out)
		return (NULL);
	i = 0;
	k = 0;
	while (line[i])
	{
		if (line[i] == '<' || line[i] == '>')
			add_redir_char(out, line, &i, &k);
		else
			out[k++] = line[i];
		i++;
	}
	out[k] = '\0';
	return (out);
}

int	handle_redirections_loop(char **args, char **envp, int last_exit,
		int *last_hd)
{
	int	i;
	int	rc;

	i = 0;
	while (args && args[i])
	{
		rc = exec_redir_token(args, i, envp, last_exit);
		if (rc == -1 || rc == -2)
			return (rc);
		if (rc > 1)
		{
			*last_hd = rc;
			continue ;
		}
		if (rc == 1)
			continue ;
		i++;
	}
	return (0);
}
