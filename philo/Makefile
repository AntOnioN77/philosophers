NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LFLAGS = -lpthread

SOURCES = create_mutexes.c init_philosophers.c observer.c philo_routine_utils.c\
philo_routine.c validate_args_utils.c validate_args.c ft_str.c ft_lltoa.c\
main.c observer_utils.c free_simulated_world.c find_left_fork.c

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
