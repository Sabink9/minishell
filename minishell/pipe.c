/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 20:00:31 by sab               #+#    #+#             */
/*   Updated: 2025/11/11 15:42:30 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

static char	**prepare_argv(char **cmd, char **envp, int last_status)
{
	char	**argv2;
	int		rc;

	rc = handle_redirections(cmd, envp, last_status);
	if (rc != 0)
	{
		if (rc == -2)
			exit(130);
		exit(1);
	}
	argv2 = compact_argv(cmd);
	if (!argv2 || !argv2[0])
		exit(0);
	return (argv2);
}

/* export + unset */
static int	dispatch_env_builtins(char **argv2, char **envp)
{
	int		code;
	char	**tmp;
	char	**new_env;

	tmp = ft_env_dup(envp);
	if (!tmp)
		return (1);
	if (!ft_strcmp(argv2[0], "export"))
	{
		new_env = ft_export(tmp, argv2, &code);
		if (new_env && new_env != tmp)
			free_split(new_env);
		else
			free_split(tmp);
		return (code);
	}
	if (!ft_strcmp(argv2[0], "unset"))
	{
		code = ft_unset(argv2, &tmp);
		free_split(tmp);
		return (code);
	}
	free_split(tmp);
	return (-1);
}

/* dispatcher principal */
int	builtin_dispatch(char **argv2, char **envp)
{
	int	code;

	if (!ft_strcmp(argv2[0], "exit"))
	{
		code = 0;
		ft_exit(argv2, &code, 1);
		return (code);
	}
	if (!ft_strcmp(argv2[0], "echo"))
		return (ft_echo(argv2));
	if (!ft_strcmp(argv2[0], "pwd"))
		return (ft_pwd());
	if (!ft_strcmp(argv2[0], "env"))
		return (ft_env(argv2, envp));
	return (dispatch_env_builtins(argv2, envp));
}

static void	exec_external(char **argv2, char **envp)
{
	char	*path;

	path = find_executable(argv2[0], envp);
	if (!path)
	{
		if (ft_strchr(argv2[0], '/'))
			exit(126);
		write(2, "minishell: command not found: ", 30);
		write(2, argv2[0], ft_strlen(argv2[0]));
		write(2, "\n", 1);
		exit(127);
	}
	execve(path, argv2, envp);
	perror(argv2[0]);
	exit(126);
}

void	child_process(char **cmd, char **envp, int last_status)
{
	char	**argv2;
	int		code;

	setup_child_signals();
	argv2 = prepare_argv(cmd, envp, last_status);
	code = builtin_dispatch(argv2, envp);
	if (code >= 0)
		exit(code);
	exec_external(argv2, envp);
}
