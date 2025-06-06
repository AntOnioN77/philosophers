/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:55:51 by antofern          #+#    #+#             */
/*   Updated: 2025/05/21 13:25:09 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <limits.h>
# include <stdio.h>

typedef int	t_bool;

# define TEST 0
# define DELAY_FACTOR 3
# define OBSERVER_PAUSE 5
# define RECALL_WAIT 500
# define FALSE 0
# define TRUE 1
# define ODD 1
# define EVEN 2
# define ERROR 2

enum	e_argx
{
	NUM_OF_PHILO,
	TIME_TO_DIE,
	TIME_TO_EAT,
	TIME_TO_SLEEP,
	MAX_EATS,
};

typedef enum e_states
{
	NO_EAT,
	ALMOST_EAT,
	YES_EAT,
	EATING,
	THE_END,
}	t_states;

/*STRUCTS*/
typedef struct s_world
{
	unsigned int	argx[5];
	long long		*dead_date_arr;
	pthread_mutex_t	*dead_date_mutex_arr;
	pthread_t		*philosophers;
	long long		start_date;
	pthread_t		observer;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*mutex_state_array;
	t_states		*state_array;
	unsigned int	tinking_time;
}	t_world;

typedef struct s_scope
{
	int				name;
	unsigned int	*argx;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	long long		start_date;
	long long		*dead_date;
	pthread_mutex_t	*dead_date_mutex;
	t_states		*state;
	pthread_mutex_t	*mutex_state;
	unsigned int	tinking_time;
}	t_scope;

int				validate_args(int argc, char **argv);
void			*philo_routine(void *sc);
int				create_mutexes(t_world *world);
int				init_philosophers(t_world *world);
int				free_simulated_world(int sim_ret, t_world *world);
int				parse_args(int argc, char *argv[], unsigned int argx[]);
char			*ft_lltoa(long long n);
void			*observer_routine(void *world);
long			ft_atol(const char *str);
void			ft_putstr_fd(char *s, int fd);
int				monitor(t_scope *scp, long long start, char *msg);
char			*ft_strjoin(char const *s1, char const *s2);
char			*cmpmsg(long long start, long long time, unsigned int name,
					char *msg);
long long		get_time_ms(void);
void			destroy_arr_mutex(pthread_mutex_t forks[], unsigned int count);
t_bool			valid_amount_ms(char *num);
t_bool			is_in_bounds(char *num);
t_scope			*scope_of_this_philo(t_world *world,
					unsigned int philo_n);
pthread_mutex_t	*find_left_fork(pthread_mutex_t *forks, int philo_n,
					unsigned int total_philo);
size_t			ft_strlen(const char *s);
char			*join_check(char *str_a, char *str_b, char **result);
void			end_all_philos(pthread_mutex_t *mutex_state_array,
					t_states *state_array, unsigned int num_of_philos);
void			establish_order(int *arr, int prev, int this, int next);
int				free_simulated_world(int sim_ret, t_world *world);
#endif
