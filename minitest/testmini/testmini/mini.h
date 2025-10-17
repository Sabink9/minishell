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

/* ----------------- UTILS ----------------- */
void free_split(char **split);

#endif