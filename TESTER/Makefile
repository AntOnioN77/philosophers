NAME = runt_test
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3

SOURCES = data_verifier.c

OBJECTS = $(SOURCES:.c=.o)
HEADERS =

all: $(NAME)
$(NAME): $(OBJECTS) $(HEADERS)

	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJECTS)
fclean: clean
	rm -f $(NAME)
re: fclean all
test: $(NAME)
	./$(NAME)
.PHONY: clean fclean all