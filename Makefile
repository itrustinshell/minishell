NAME = minishell

CC = gcc

CFLAGS = -g -gdwarf-4 -Wall -Wextra -Werror

SRC =   tokenizer/fill_tokenmatrix.c tokenizer/get_tokens_number.c tokenizer/tokenizer.c\
		builtins/ft_export.c builtins/ft_export_utils.c builtins/litoma.c builtins/ft_unset.c builtins/ft_env.c builtins/ft_env_lists.c builtins/ft_cd.c builtins/ft_echo.c builtins/ft_exit.c builtins/ft_pwd.c parser/parser.c parser/parser_utils.c\
		executor/executor.c executor/pipex.c executor/builtinex.c executor/heredoc.c executor/heredoc_list.c executor/heredoc_utils.c executor/redirutils.c executor/redirops.c executor/pipex_utils.c\
		test/pipe_test.c test/printmatrix.c\
		utils/free.c utils/list_utils.c utils/cmd_utils.c utils/init.c utils/general_utils.c\
		main.c \

OBJ = $(SRC:.c=.o)

all: libft $(NAME)

libft:
	$(MAKE) bonus -C libft

%.o: %.c
	$(CC) $(CFLAGS) -I./libft -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -L./libft -lft -lreadline -o $(NAME)

clean:
	rm -f $(OBJ)
	$(MAKE) -C libft clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C libft fclean

re: fclean all

release: CFLAGS := -Wall -Wextra -Werror
release: fclean $(NAME)

.PHONY: all clean fclean re release libft
