/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philosophers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:49:28 by antofern          #+#    #+#             */
/*   Updated: 2025/05/19 13:31:30 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int			init_one_philo(t_world *world, unsigned int philo_n);
static int	reserve_memory(t_world *world, unsigned int num_of_philos);

int	init_philosophers(t_world *world)
{
	unsigned int	n_philos;
	unsigned int	current;
	int				error;

	n_philos = world->argx[NUM_OF_PHILO];
	if (reserve_memory(world, n_philos))
		return (1);
	world->start_date = get_time_ms();
	current = 1;
	error = 0;
	while (current <= n_philos)
	{
		error = init_one_philo(world, current);
		if (error)
		{
			current--;
			while (current > 0)
				pthread_join(world->philosophers[current - 1], NULL);
			free(world->philosophers);
			free(world->dead_date_arr);
			return (error);
		}
		current++;
	}
	return (0);
}

static int	reserve_memory(t_world *world, unsigned int num_of_philos)
{
	world->philosophers = malloc(sizeof(pthread_t) * num_of_philos);
	if (world->philosophers ==NULL)
		return (1);
	world->dead_date_arr = malloc(sizeof(long long) * num_of_philos);
	if (world->dead_date_arr == NULL)
	{
		free(world->philosophers);
		return (1);
	}
	world->state_array = malloc(sizeof(t_states) * num_of_philos);
	if (world->state_array == NULL)
	{
		free(world->philosophers);
		free(world->dead_date_arr);
		return (1);
	}
	memset(world->state_array, 0, sizeof(t_states) * num_of_philos);
	return (0);
}

int	init_one_philo(t_world *world, unsigned int philo_n)
{
	pthread_t 		*new_philo;
	t_scope	*scope;

	philo_n--;

	world->dead_date_arr[philo_n] = (world->argx[TIME_TO_DIE]) + world->start_date;
	scope = scoop_of_this_philo(world, philo_n);
	if( scope == NULL)
		return (1);
	new_philo = &(world->philosophers[philo_n]);
	
	if(pthread_create(new_philo, NULL, philo_routine, scope))
		return (1);
	return (0);

}


//cambiar nombre, establece dos cosas, que tenedor usara primero pero tambien, si al inicio tendra permiso para comer.
//TODO abdtraer la logica izquierda derecha y resolver todo lo relativo a los tenedores en esta funcion
void	stablish_order_forks(unsigned int name, t_scope *scope, pthread_mutex_t *left,
	pthread_mutex_t *rigth)
{
	if ((name % 2) == 0)
	{
		scope->first_fork = left;
		scope->second_fork = rigth;
		*(scope->state) = NO_EAT;
	}
	else
	{
		scope->first_fork = rigth;
		scope->second_fork = left;
		*(scope->state) = YES_EAT;
	}
}

t_scope	*scoop_of_this_philo(t_world *world, unsigned int philo_n)
{
	t_scope	*scope;
	pthread_mutex_t	*left;
	pthread_mutex_t	*rigth;

	scope = malloc(sizeof(t_scope));
	if (scope == NULL)
		return (NULL);
	scope->name = philo_n + 1;
	scope->argx = world->argx;
	left = find_left_fork(world->forks, philo_n, world->argx[NUM_OF_PHILO]);
	rigth = &(world->forks[philo_n]);

	scope->state = &(world->state_array[philo_n]);
	scope->mutex_state = &(world->mutex_state_array[philo_n]);
	stablish_order_forks(scope->name, scope, left, rigth);

	scope->start_date = world->start_date;
	scope->dead_date = &(world->dead_date_arr[philo_n]);
	scope->dead_date_mutex = &(world->dead_date_mutex_arr[philo_n]);

	scope->tinking_time =world->tinking_time;//  ((world->argx[TIME_TO_EAT] - world->argx[TIME_TO_SLEEP]) * 1000);
	return (scope);
}

pthread_mutex_t	*find_left_fork(pthread_mutex_t *forks, int philo_n, unsigned int total_philo)
{
	if(total_philo == 1)
		return (NULL);
	if(philo_n == 0)
		return (&(forks[total_philo -1]));
	else
		return (&(forks[philo_n -1]));
}
