#include "mini.h"

int_ft_pwd(void)
{
	char	*cwd;

	cdw = getcdw(NULL, 0);
	if (cdw == NULL)
	{
		perror("pwd");
		return(1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);	
}