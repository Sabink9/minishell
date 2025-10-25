#ifndef MINI_H
# define MINI_H

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../libft/libft.h"

// typedef struct s_env
// {
// 	char			*key;
// 	char			*value;
// 	struct s_env	*next;
// }	t_env;

char **ft_split(const char *s);
int ft_pwd(void);
char **ft_export(char **envp, char **args);
void ft_echo(char **args);
char	**ft_cd(char **args, char **envp);

/* ----------------- EXEC / PATH ----------------- */
char *get_path_from_env(char **envp);
char *find_executable(char *cmd, char **envp);
void exec_command(char **args, char **envp);
int	exec_piped_commands(char ***cmdv, int n, char **envp);
char	***parse_pipes(char **split, int *n);
void	free_cmdv(char ***cmdv);

/* ----------------- UTILS ----------------- */
void free_split(char **split);
int	ft_strcmp(char *s1, char *s2);

int	handle_redirections(char **args);
int	restore_std_fds(int saved_in, int saved_out);
char	*expand_variables(char *line, char **envp, int last_exit);
char	*process_quotes(char *line);

#endif