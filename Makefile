NAME = philosophers
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LFLAGS = -lpthread

SOURCES = create_mutexes.c init_philosophers.c observer.c parse_args.c\
philo_routine.c valid_amount_ms.c validate_args.c utils.c ft_lltoa.c main.c

OBJECTS = $(SOURCES:.c=.o)
HEADERS = philosophers.h

all: $(NAME)
$(NAME): $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LFLAGS)
%.o: %.c $(HEADERS) 
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJECTS)
fclean: clean
	rm -f $(NAME)
re: fclean all
.PHONY: clean fclean all