#include "../libft/libft.h"
#include "mini.h"

int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
int	ft_env(char **args, char **envp)
{
	int	i;

	// 🔹 Vérifie s'il y a un vrai argument (pas une redirection)
	if (args[1]
		&& ft_strcmp(args[1], ">") != 0
		&& ft_strcmp(args[1], ">>") != 0
		&& ft_strcmp(args[1], "<") != 0
		&& ft_strcmp(args[1], "<<") != 0)
	{
		printf("minishell: env: %s: No such file or directory\n", args[1]);
		return (127);
	}

	// 🔹 Affiche toutes les variables d'environnement
	i = 0;
	while (envp && envp[i])
	{
		if (ft_strchr(envp[i], '='))
			printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

int	ft_is_numeric(const char *s)
{
	int	i;

	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args, int *exit_status, int is_child)
{
	long	code;

	if (!is_child)
		write(1, "exit\n", 5);
	if (args[1] && !ft_is_numeric(args[1]))
	{
		ft_putstr_fd2("minishell: exit: ");
		ft_putstr_fd2(args[1]);
		ft_putstr_fd2(": numeric argument required\n");
		exit(2);
	}
	if (args[1] && args[2])
	{
		ft_putstr_fd2("minishell: exit: too many arguments\n");
		*exit_status = 1;
		return (1);
	}
	if (args[1])
		code = ft_atol(args[1]);
	else
		code = *exit_status;
	exit((unsigned char)code);
}
