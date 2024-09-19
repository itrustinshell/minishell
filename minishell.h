#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

/*-------------is_space (these are the ASCII values)..............*/
#define SAPCE 32
#define TABULATION 9
#define NEW_LINE 10
#define IS_SPACE 1
#define IS_NOT_SPACE 0

/*-------------split--------------------------------------------*/
#define LOCKED 1
#define UNLOCKED 0


typedef struct s_token
{
	char	*str;
	struct s_token *next;
	struct s_token *prev;

} t_token;

char **ft_split(char *str_to_split);
