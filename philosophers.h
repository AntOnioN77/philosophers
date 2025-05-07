#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

# include <pthread.h>
# include <unistd.h>
# include "libft/headers/libft.h"
# include "libft/headers/parsing.h"
# include <string.h>
# include <sys/time.h>
# include <limits.h>

# define ODD 1
# define EVEN 2

/*LOCATION: validate_args.c*/
int validate_args(int argc, char **argv);
void *even_philo(void *sc);
void *odd_philo(void *sc);

void	destroy_arr_mutex(pthread_mutex_t forks[], int count);

/*STRUCTS*/
typedef struct s_world
{
	int             argx[5];
	long long       *dead_arr;		// array
	pthread_mutex_t	*dead_mutex_arr;	// array
	pthread_t 		*philosophers;	// array
	pthread_t       observer;
	pthread_mutex_t	*forks;			// array
	pthread_mutex_t mutex_end;
	int             the_end;
}   t_world;

typedef struct s_philo_scope
{
	int				name;
	int				*argx;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	long long		*dead;
	pthread_mutex_t	*dead_mutex;
	int				*the_end;
	pthread_mutex_t *mutex_end;
}   t_philo_scope;

#endif