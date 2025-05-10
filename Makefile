NAME = philosophers
CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS =
DBGFLAGS = -g3

SOURCES = create_mutexes.c init_philosophers.c observer.c parse_args.c philo_routine.c valid_amount_ms.c validate_args.c

OBJECTS = $(SOURCES:.c=.o)
HEADERS = philosophers.h

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
.PHONY: clean fclean all