/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:33:27 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/12 20:01:43 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

static void	handle_simple_builtins(char **argv, char **envp, int *exit_status)
{
	if (!ft_strcmp(argv[0], "echo"))
		*exit_status = ft_echo(argv);
	else if (!ft_strcmp(argv[0], "pwd"))
		*exit_status = ft_pwd();
	else if (!ft_strcmp(argv[0], "env"))
		*exit_status = ft_env(argv, envp);
	else if (!ft_strcmp(argv[0], "exit"))
		ft_exit(argv, exit_status, 0);
}

static void	handle_cd_builtin(char **argv, char **envp, int *exit_status)
{
	if (argv[1] && argv[2])
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		*exit_status = 1;
	}
	else
		*exit_status = ft_cd(argv, envp);
}

static char	**handle_env_builtins(char **argv, char **envp, int *exit_status)
{
	char	**new_env;

	if (!ft_strcmp(argv[0], "export"))
	{
		new_env = ft_export(envp, argv, exit_status);
		if (new_env && new_env != envp)
			envp = new_env;
	}
	else if (!ft_strcmp(argv[0], "cd"))
		handle_cd_builtin(argv, envp, exit_status);
	else if (!ft_strcmp(argv[0], "unset"))
		*exit_status = ft_unset(argv, &envp);
	else
		*exit_status = exec_command(argv, envp);
	return (envp);
}

char	**exec_builtin_or_cmd(char **argv, char **envp, int *exit_status)
{
	if (!ft_strcmp(argv[0], "echo") || !ft_strcmp(argv[0], "pwd")
		|| !ft_strcmp(argv[0], "env") || !ft_strcmp(argv[0], "exit"))
	{
		handle_simple_builtins(argv, envp, exit_status);
		return (envp);
	}
	return (handle_env_builtins(argv, envp, exit_status));
}

char	**handle_pipes(char **split, char **envp, int *exit_status)
{
	char	***cmdv;
	int		cmd_count;

	cmdv = parse_pipes(split, &cmd_count);
	if (!cmdv)
	{
		*exit_status = 2;
		g_sig = 0;
		return (envp);
	}
	*exit_status = exec_piped_commands(cmdv, cmd_count, envp, *exit_status);
	free_cmdv(cmdv);
	return (envp);
}
