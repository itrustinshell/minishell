#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>  // For pid_t
#include <sys/wait.h>   // For wait()

/*----colori-----*/
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

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

/*-----------pipe_logic----*/
#define THERE_IS_A_PIPE 1
#define THERE_IS_NOT_A_PIPE 0
#define THERE_IS_A_REDIRECTION 1
#define THERE_IS_NOT_A_REDIRECTION 0
#define READ_HERE_STDIN 0
#define WRITE_HERE_STDOUT 1
#define CHILD_PID 0
#define CHILD_PID0 0
#define READ_END 0
#define WRITE_END 1

/*----------path_check------*/
#define FILE_DOESNT_EXIST -1

/**/

#define LOCKED 1
#define UNLOCKED 0

/*****type:f:Redir*****/

#define OUT_REDIR 1
#define INPUT_REDIR 2
#define APPEND_REDIR 3
#define OTHER 4

typedef struct s_redir
{
	int type_of_redir; //vedi le macro
	char *outredir_file;
	//TODO inserire altri campi per le specificità delle altre redir
	struct	s_redir	*prev;
	struct 	s_redir *next;
} t_redir;

typedef struct s_command
{
	char				*cmd;
	char				**args;
	char				*path;
	struct s_command	*next;
	struct s_command	*prev;
	t_redir		*redirlist;
} t_command;

typedef struct s_token
{

	char	*str;
	struct s_token *next;
	struct s_token *prev;

} t_token;




/*-----tests---------*/
void test_stampa_args(t_command *commandlist);
void print_matrix_of_char(char **matrix);
void print_matrix_of_int(int **matrix, int limit_row, int limit_column);
void print_list(t_command *commandlist);

/*-----tokenizer------*/
char **tokenizer(char *str_to_tokenize);
int get_num_of_tokens(char *str);
int ft_isspace(char char_to_check);
int ft_issymbol(char char_to_check);
int check_symbols(char *str_tocheck, int *iterator);
char **create_tokenmatrix(char* str_to_tokenize, int n_tokens);

/*-----pipe---------*/
//t_command *commandlist_for_pipe(char **matrix);
int **generate_array_of_pipes_with_fd(int num_of_cmd);
int pipex(t_command *cmdlist, int cmdlist_len, int **pipesarray);
t_command *	commandlist_for_pipe(char **matrix);


/*---------redir----------*/
t_redir *redirlist_for_pipe(char **tokenmatrix, int token_index);



/*-----pipe's utils--------*/
int **pipematrix_malloc(int cmdlist_len);
int check_pipe_symbol(char **matrix);
t_command *create_commandnode_for_pipe(char **tokenmatrix, int current_pipe_index, int current_generictoken_index);
t_command *commandlist_for_pipe(char **tokenmatrix);
void commandnode_management_for_pipe(char **tokenmatrix, int *pipe_index, int *generictoken_index, t_command **commandlist);

/*------cmd------*/
char *find_external_cmd(char *cmd);	
t_command *create_cmd(char **matrix);
void single_cmd_ex(t_command *cmd);
char *get_cmdpath(char *cmd);

/*----utils-----*/
char **ft_split(char *str, char separator);
int ft_isspace(char char_to_check);
char *strjoin(char *str, char *separator);
int listlen(t_command *list);
int ft_strlen(char *str);

//utils_lists
void listappend_command(t_command *node, t_command **list);
void listappend_redir(t_redir *node, t_redir **list);
t_command *find_last_commandnode(t_command *commandlist);
