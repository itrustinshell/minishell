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
#define APPEND_REDIRECTION 222
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

	int				type; //vedi le macro
	char			*outredir_file;
	struct s_redir	*head;
	struct s_redir	*prev;
	struct s_redir	*next;
} t_redir;

typedef struct s_command
{
	char				*cmd;
	char				**args;
	char				*path;
	struct s_command	*next;
	t_redir				*redirlist;
} t_command;

typedef struct s_token
{
	char			*str;
	struct s_token	*next;
	struct s_token	*prev;

} t_token;

typedef struct s_env
{
	char 			*name;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
} t_env;

/*-----tests---------*/
void test_stampa_args(t_command *commandlist);
void print_matrix_of_char(char **matrix);
void print_matrix_of_int(int **matrix, int limit_row, int limit_column);
void print_list(t_command *commandlist);

//TOKENIZER
char **tokenizer(char *str_to_tokenize);
int get_num_of_tokens(char *str);
int ft_isspace(char char_to_check);
int ft_issymbol(char char_to_check);
int check_symbols(char *str_tocheck, int *iterator);
char **create_tokenmatrix(char* str_to_tokenize, int n_tokens);


//PARSING
t_command 	*parsing(char **tokenmatrix);
void		listappend_command(t_command *node, t_command **list);
t_command	*last_cmdnode(t_command *commandlist);
int			listlen(t_command *list);
void 		listappend_redir(t_redir *node, t_redir **list);


//EXECUTOR


void		executor(t_command *cmdlist, t_env **env);

int			pipex(t_command *cmdlist, int cmdlist_len, int **pipesarray, t_env **env);
void		cmdex(t_command *cmd, t_env **env);
int			builtinex(t_command *cmd, t_env **env);

//executor utils
void		ft_execve(t_command *tmp_cmdlist, t_env *genvlist);
int			**generate_array_of_pipes_with_fd(int num_of_cmd);
int 		**pipesalloc(int cmdlist_len);
int 		pipecheck(char **matrix);
void		read_stdin_from_pipe(int **pipematrix, int i);
void		write_stdout_in_the_pipe(int **pipematrix, int i);
void		close_all_pipe(int **pipematrix, int cmdlist_len);
void		fork_along_pipesloop(int **pipematrix,t_command *tmp_cmdlist, int i, int cmdlist_len, t_env **env);
int			check_builtin_in_cmdlist(t_command *tmp_cmdlist, t_env *genvlist);
t_command 	*create_commandnode_for_pipe(char **tokenmatrix, int current_pipe_index, int current_generictoken_index);
void 		commandnode_management_for_pipe(char **tokenmatrix, int *pipe_index, int *generictoken_index, t_command **commandlist);
t_redir		*redirlist_for_pipe(char **tokenmatrix, int token_index);
int 		input_output_routine(t_redir *redirlist,  int saved_stdout);


//free
void ft_free_n_matrix(char **matrix, int n);
void ft_freematrix(char **matrix);
void ft_freelist(t_env *envlist);
void free_cmd(t_command *cmd);


///BUILTINS
int ft_echo(int argc, char **argv);
int ft_pwd();
int ft_cd(char **argv);
void ft_exit();
void ft_export(char *namevar, t_env **env);
int	printenvlist(t_env *lenvlist);
//builtins utils
void 	addto_globalenv(t_env **local_envlist, char *str);
int 	there_is_equal_before_end(char *str);
int 	ends_with_equal_sign(char *str);
int 	exportcheck(char **matrix);
t_env	*last_envlist(t_env *envlist);
t_env	*create_lenvnode(char *str);
void	envlist_append(t_env *node, t_env **list);
int		envlist_len(t_env *list);
t_env	*copy_envp(char **envp);
char 	*is_valid_lvar(char *lvar, char **l_envp);
char	*is_valid_lvar(char *lvar, char **l_envp);
void	init_envnode(t_env *env);
t_env	*access_envar(char *envar, t_env *envlist);


//CMD utils
char *find_external_cmd(char *cmd);	
t_command *create_cmd(char **matrix);
char *get_cmdpath(char *cmd);
void init_cmd(t_command *cmd);

//GENERAL utils
char **ft_split(char *str, char separator);
int ft_isspace(char char_to_check);
char *strjoin(char *str, char *separator);
int ft_strlen(char *str);
int matrixlen(char **matrix);
char **convert_list_to_matrix(t_env *envlist);
