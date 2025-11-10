/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saciurus <saciurus@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 17:16:12 by saciurus          #+#    #+#             */
/*   Updated: 2025/11/10 18:49:18 by saciurus         ###   ########.fr       */
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

// expand
char							*get_env_value(const char *key, char **envp);
char							*expand_exit_status(char *res, int *i,
									int last_exit);
char							*expand_env_var(char *res, char *line, int *i,
									char **envp);
char							*append_char(char *res, char c);

// free
void							free_partial(char **tab, int count);
void							free_split(char **tab);
void							free_dup_until(char **out, int i);
void							free_cmdv(char ***cmdv);

// unset
void							print_unset_error(const char *arg);
char							**env_remove_key(char **envp, const char *key);

void							setup_interactive_signals(void);
void							setup_child_signals(void);
void							sigint_handler(int s);
char							*expand_variables(char *line, char **envp,
									int last_exit);

int								ft_pwd(void);
int								ft_echo(char **split);
int								ft_cd(char **args, char **envp);

// EXEC / PATH
char							*find_executable(char *cmd, char **envp);
void							parent_ignore_signals(void);
int								exec_command(char **args, char **envp);

// pipes
int								exec_piped_commands(char ***cmdv, int n,
									char **envp, int last_status);
char							***parse_pipes(char **split, int *n);
void							child_setup_io(int in_fd, int *pipefd,
									int is_last);
void							child_process(char **cmd, char **envp,
									int last_status);
int								finalize_status(int status_last);

// handle_command
char							**handle_pipes(char **split, char **envp,
									int *exit_status);
char							**exec_builtin_or_cmd(char **argv, char **envp,
									int *exit_status);
char							**handle_command(char **envp, char **split,
									int *exit_status);

// UTILS
char							**ft_split(char *s);
int								ft_strcmp(char *s1, char *s2);
char							*ft_strjoin(char *s1, char *s2);
char							*strjoin_char_free(char *s, char c);
char							*strjoin_free(char *s1, char *s2);
void							skip_args(char **args, int *i);
int								is_valid_identifier(const char *s);

// redir
int								redir_in(const char *file);
int								redir_out(const char *file, int append);
int								handle_heredoc(char *delim, char **envp,
									int last_exit);
int								handle_redirections(char **args, char **envp,
									int last_exit);
int								restore_std_fds(int saved_in, int saved_out);
char							*process_quotes(char *line);
void							remove_n_tokens(char **args, int pos, int n);
int								hd_setup_pipe(int pfd[2], char *delim,
									char **clean, int *do_expand);
int								hd_handle_line(int wfd, char *line, char **envp,
									int *cfg);
int								apply_heredoc(char **args, int i, char **envp,
									int last_exit);
int								finish_file_redir(char *fname, int is_in,
									int append);

// export_utils1.c
int								envp_size(char **envp);
int								find_env_index(char **envp, const char *key);
void							print_export(char **envp);
char							*make_env_var(const char *key,
									const char *value);

// export_utils2.c
char							**append_env_var(char **envp, char *new_var,
									int size);
char							**set_env_var(char **envp, const char *key,
									const char *value);
void							split_key_value(const char *str, char **key,
									char **value);
char							**ft_export(char **envp, char **argv,
									int *exit_status);
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

void							print_args(char **argv, int i);
char							*unquote_token(const char *s);
char							**init_env(char **envp_sys);
int								handle_empty_or_signal(char *line,
									int *exit_status);
char							**process_line(char *line, char **envp,
									int *exit_status);
#endif
