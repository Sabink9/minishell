#include "mini.h"

int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("pwd");
		return(1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);	
}