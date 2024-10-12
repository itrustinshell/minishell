#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int		pipe_array[2];
	
	if (pipe(pipe_array) < 0)
	{
		perror("minishell");
		return (1);
	}
	printf("this is first pid of pipe: %d\n", pipe_array[0]);
	printf("this is second pid of pipe: %d\n", pipe_array[1]);

//I have two file descriptor

	char msg[] = "ciaoo\n";
//scrivi in 
	write(pipe_array[1], msg,sizeof(msg));
	


// let's read
	char read_buffer[100];
//leggi da...e metti in rea_buffer
	int a = read(pipe_array[0], read_buffer, sizeof(msg));

	printf("this is msg form read_buffer: %s\n", read_buffer);
printf("il numero di caratteri letti è: %d\n", a);

}
