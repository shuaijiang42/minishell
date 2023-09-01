#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	main(int argc, char **argv, char **env)
{
	extern char **environ;
	char *str;
	env[1] = strdup("HOLA=\"QUIEO\"");
	printf("%s", environ[1]);
	return (0);

}
