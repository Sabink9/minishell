/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:45:34 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/12 20:00:58 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "mini.h"

char	**init_env(char **envp_sys)
{
	char	**envp;

	envp = ft_env_dup(envp_sys);
	if (!envp)
	{
		write(2, "minishell: failed to init env\n", 30);
		exit(1);
	}
	setup_interactive_signals();
	return (envp);
}

int	handle_empty_or_signal(char *line, int *exit_status)
{
	if (g_sig == SIGINT)
	{
		*exit_status = 130;
		g_sig = 0;
		return (1);
	}
	if (*line == '\0')
		return (1);
	return (0);
}

char	**process_line(char *line, char **envp, int *exit_status)
{
	char	**split;
	char	*tmp;

	add_history(line);
	tmp = process_quotes(line);
	free(line);
	line = tmp;
	tmp = expand_variables(line, envp, *exit_status);
	free(line);
	line = tmp;
	tmp = preprocess_redirs(line);
	free(line);
	line = tmp;
	split = ft_split(line);
	if (split && split[0])
		envp = handle_command(envp, split, exit_status);
	free_split(split);
	free(line);
	return (envp);
}
