NAME= philo
CC= cc
CFLAGS= -Wall -Wextra -Werror -g
SRC= src/main.c \

OBJ= $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) 
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all lib re clean fclean
