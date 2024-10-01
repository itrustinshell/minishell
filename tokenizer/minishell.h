#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>




/*-------------is_space (these are the ASCII values)..............*/
#define SAPCE 32
#define TABULATION 9
#define NEW_LINE 10
#define IS_SPACE 1
#define IS_NOT_SPACE 0

/*-----------issymbol----------*/
#define INPUT_REDIRECTION 60 //left arrow
#define OUTPUT_REDIRECTION 62 //right arrow
#define PIPE 124
#define DOLLAR_SIGN 36
#define SINGLE_QUOTE 39
#define DOUBLE_QUOTE 34
#define IS_SYMBOL 1
#define IS_NOT_SYMBOL 0
/*-------------split--------------------------------------------*/
#define LOCKED 1
#define UNLOCKED 0


typedef struct s_token
{
	char	*str;
	struct s_token *next;
	struct s_token *prev;

} t_token;



/*-----tokenizer------*/
char **tokenizer(char *str_to_tokenize);
int get_num_of_tokens(char *str);
int ft_isspace(char char_to_check);
int ft_issymbol(char char_to_check);
int check_symbols(char *str_tocheck, int *iterator);
char **create_tokenmatrix(char* str_to_tokenize, int n_tokens);
