#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	char	*cwd_buffer;

	cwd_buffer = NULL;
	cwd_buffer = getcwd(NULL, 0);
	printf("%s\n", cwd_buffer);



	free(cwd_buffer);



	return (0);
}
