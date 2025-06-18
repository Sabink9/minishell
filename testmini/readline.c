#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char	*line;

	line = readline(">> ");
	if (line)
	{
		printf("Tu as tapé : %s\n", line);
		free(line);
	}
	return (0);
}
