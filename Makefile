NAME = minishell

CC = gcc

#CFLAGS = -Wall -Wextra -Werror

SRC =   tokenizer/fill_tokenmatrix.c tokenizer/get_tokens_number.c tokenizer/tokenizer.c \
		pipe/ft_pipe.c pipe/ft_pipe_utils.c\
		env/env_lists.c env/env_utils.c env/env.c env/ft_export.c\
		test/pipe_test.c test/printmatrix.c\
		utils/ft_split.c utils/list_utils.c utils/ft_isspace.c utils/strjoin.c utils/ft_strlen.c \
		cmd/cmd_utils.c cmd/manage_single_cmd.c\
		main.c \

OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

# Clean object files and MiniLibX
clean:
	rm -f $(OBJ)

# Clean everything, including the final executable
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

# Build with optimizations (use this for release, removes AddressSanitizer)
release: CFLAGS := -Wall -Wextra -Werror
release: fclean $(NAME)

all: $(NAME)

