#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

# include <pthread.h>
# include <unistd.h>
# include "libft/headers/libft.h"
# include "libft/headers/parsing.h"
#include <string.h>

# define ODD 1
# define EVEN 2

/*LOCATION: validate_args.c*/
int validate_args(int argc, char **argv);

/*STRUCTS*/
typedef struct s_world
{
    int             argx[5];
    long long       *dead_arr;//
	pthread_t 		**philosophers;
    pthread_t       *observer;
	pthread_mutex_t	**forks;
    int             *the_end;
}   t_world;

typedef struct s_philo_scope
{
    int             name;
    int             *argx;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    long long       *dead;
    int             *the_end;
}   t_philo_scope;

#endif