#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

# include <pthread.h>
# include <unistd.h>
# include "libft/headers/libft.h"
# include "libft/headers/parsing.h"
#include <string.h>

/*LOCATION: validate_args.c*/
int validate_args(int argc, char **argv);
int	init_forks(t_world *world);

/*STRUCTS*/
typedef struct s_world
{
    int             argx[5];
    int				*forks;
	pthread_t 		*philosophers;
    pthread_t       *observer;
	pthread_mutex_t	*mute_arr;
}   t_world;

#endif