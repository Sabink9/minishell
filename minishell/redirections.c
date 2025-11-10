/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:41 by sab               #+#    #+#             */
/*   Updated: 2025/11/10 18:25:13 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

/* -- "<file", ">file", ">>file" collés -- */
static int	process_inline_redir(char **args, int i)
{
	int		append;
	char	*raw;
	char	*fname;

	append = 0;
	if (args[i][0] == '<')
		raw = args[i] + 1;
	else if (args[i][1] == '>')
	{
		append = 1;
		raw = args[i] + 2;
	}
	else
		raw = args[i] + 1;
	if (!*raw)
		return (0);
	fname = unquote_token(raw);
	if (!fname)
		return (-1);
	if (finish_file_redir(fname, args[i][0] == '<', append) < 0)
		return (-1);
	remove_n_tokens(args, i, 1);
	return (1);
}

/* gère ">" et ">>" */
/* gère ">" et ">>" (args[0] = "<" / ">" / ">>") */
static int	fsfs(char **tok, char *fname)
{
	int	append;

	append = 0;
	if (!ft_strcmp(tok[0], ">>"))
		append = 1;
	return (finish_file_redir(fname, 0, append));
}

/* "< token", "> token", ">> token" séparés */
static int	process_pure_redir(char **args, int i, char **envp, int last_exit)
{
	char	*fname;

	if (!ft_strcmp(args[i], "<<"))
		return (apply_heredoc(args, i, envp, last_exit));
	if (!args[i + 1] || !args[i + 1][0])
	{
		write(2,
			"minishell: syntax error near unexpected token `newline'\n", 57);
		return (-1);
	}
	fname = unquote_token(args[i + 1]);
	if (!fname)
		return (-1);
	if (!ft_strcmp(args[i], "<"))
	{
		if (finish_file_redir(fname, 1, 0) < 0)
			return (-1);
	}
	else if (fsfs(&args[i], fname) < 0)
		return (-1);
	remove_n_tokens(args, i, 2);
	return (1);
}

/* vérifie et exécute une redirection sur un token */
static int	exec_redir_token(char **args, int i, char **envp, int last_exit)
{
	int	rc;

	if (!ft_strcmp(args[i], "<") || !ft_strcmp(args[i], ">")
		|| !ft_strcmp(args[i], ">>") || !ft_strcmp(args[i], "<<"))
		rc = process_pure_redir(args, i, envp, last_exit);
	else if (args[i][0] == '<' || args[i][0] == '>')
		rc = process_inline_redir(args, i);
	else
		rc = 0;
	return (rc);
}

/* boucle principale qui parcourt et gère toutes les redirections */
int	handle_redirections(char **args, char **envp, int last_exit)
{
	int	i;
	int	rc;

	i = 0;
	while (args && args[i])
	{
		if ((ft_strchr(args[i], '\'') || ft_strchr(args[i], '"'))
			&& !(args[i][0] == '<' || args[i][0] == '>'))
		{
			i++;
			continue ;
		}
		rc = exec_redir_token(args, i, envp, last_exit);
		if (rc == -2 || rc == -1)
			return (rc);
		if (rc == 1)
			continue ;
		i++;
	}
	return (0);
}
