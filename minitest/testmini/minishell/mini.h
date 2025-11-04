/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 17:16:12 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/04 16:14:16 by saciurus         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_H
# define MINI_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_sig;

void							setup_interactive_signals(void);
void							setup_child_signals(void);
void							sigint_handler(int s);

int								ft_pwd(void);
int								ft_echo(char **split);
int								ft_cd(char **args, char **envp);

/* ----------------- EXEC / PATH ----------------- */
char							*get_path_from_env(char **envp);
char							*find_executable(char *cmd, char **envp);
int								exec_command(char **args, char **envp);
int								exec_piped_commands(char ***cmdv, int n,
									char **envp, int last_status);
char							***parse_pipes(char **split, int *n);
void							free_cmdv(char ***cmdv);

/* ----------------- UTILS ----------------- */
char							**ft_split(char *s);
void							free_split(char **tab);
int								ft_strcmp(char *s1, char *s2);
char							*ft_strjoin(char *s1, char *s2);
char							*strjoin_char_free(char *s, char c);
char							*strjoin_free(char *s1, char *s2);
void							skip_args(char **args, int *i);

// redir
int								redir_in(const char *file);
int								redir_out(const char *file, int append);
int								handle_heredoc(char *delim, char **envp,
									int last_exit);
int								handle_redirections(char **args, char **envp,
									int last_exit);
int								restore_std_fds(int saved_in, int saved_out);
char							*expand_variables(char *line, char **envp,
									int last_exit);
char							*process_quotes(char *line);

// export_utils1.c
int								envp_size(char **envp);
int								find_env_index(char **envp, const char *key);
void							print_export(char **envp);
char	*make_env_var(const char *key,
					const char *value);

// export_utils2.c
char							**append_env_var(char **envp, char *new_var,
									int size);
char							**set_env_var(char **envp, const char *key,
									const char *value);
void							split_key_value(const char *str, char **key,
									char **value);
char							**ft_export(char **envp, char **args);
char							*read_full_line(void);
int								has_pipe(char **split);
int								ft_env(char **args, char **envp);

int								ft_exit(char **args, int *exit_status,
									int is_child);
int								ft_is_numeric(const char *s);
void							ft_putstr_fd2(char *s);
long							ft_atol(const char *str);
char							**compact_argv(char **args);
int								ft_unset(char **args, char ***penvp);
char							**ft_env_dup(char **envp);
#endif