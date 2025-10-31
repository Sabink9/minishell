#ifndef MINI_H
# define MINI_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

// typedef struct s_env
// {
// 	char			*key;
// 	char			*value;
// 	struct s_env	*next;
// }	t_env;

int		ft_pwd(void);
void	ft_echo(char **split);
char	**ft_cd(char **args, char **envp);

/* ----------------- EXEC / PATH ----------------- */
char	*get_path_from_env(char **envp);
char	*find_executable(char *cmd, char **envp);
void	exec_command(char **args, char **envp);
int		exec_piped_commands(char ***cmdv, int n, char **envp);
char	***parse_pipes(char **split, int *n);
void	free_cmdv(char ***cmdv);

/* ----------------- UTILS ----------------- */
char	**ft_split(const char *s);
void	free_split(char **split);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strjoin(char *s1, char *s2);
char	*strjoin_char_free(char *s, char c);
char	*strjoin_free(char *s1, char *s2);
void	skip_args(char **args, int *i);

// redir
int redir_in(const char *file);
void	redir_out(const char *file, int append);
int		handle_redirections(char **args);
int		restore_std_fds(int saved_in, int saved_out);
char	*expand_variables(char *line, char **envp, int last_exit);
char	*process_quotes(char *line);

// export_utils1.c
int		envp_size(char **envp);
int		find_env_index(char **envp, const char *key);
void	print_export(char **envp);
char	*make_env_var(const char *key, const char *value);

// export_utils2.c
char	**append_env_var(char **envp, char *new_var, int size);
char	**set_env_var(char **envp, const char *key, const char *value);
void	split_key_value(const char *str, char **key, char **value);
char	**ft_export(char **envp, char **args);

int	unclosed_quote(const char *line);
char	*read_full_line(void);
int	has_pipe(char **split);

#endif