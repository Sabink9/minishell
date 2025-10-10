#ifndef MINI_H
# define MINI_H

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

char	**ft_split(const char *s);
int	ft_pwd(void);
t_env	*ft_export(t_env *env, char **args);
void	free_env_list(t_env *head);

#endif