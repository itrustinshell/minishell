NAME = minishell

CC = gcc
# Common flags for all builds
#CFLAGS = -Wall -Wextra -Werror

SRC =   tokenizer/fill_tokenmatrix.c tokenizer/get_tokens_number.c tokenizer/tokenizer.c \
		pipe/ft_pipe.c \
		test/pipe_test.c \


OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Main executable target, also depends on the MiniLibX library
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

