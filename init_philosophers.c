/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philosophers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:49:28 by antofern          #+#    #+#             */
/*   Updated: 2025/05/12 17:07:10 by antofern         ###   ########.fr       */
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
	current = 1;
	error = 0;
	while (current <= n_philos)
	{
		error = init_one_philo(world, current);
		if (error)
		{
			//world->the_end = 1 - world->argx[NUM_OF_PHILO];
			current--;
			while (current > 0)
				pthread_join(world->philosophers[current -1], NULL);
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
	world->philosophers =  malloc(sizeof(pthread_t) * num_of_philos);
	if (world->philosophers ==NULL)
		return (1);
	world->dead_date_arr =  malloc(sizeof(long long) * num_of_philos);
	if (world->dead_date_arr == NULL)
	{
		free(world->philosophers);
		return (1);
	}
	world->the_end_array =  malloc(sizeof(long long) * num_of_philos);
	if (world->the_end_array == NULL)
	{
		free(world->philosophers);
		free(world->dead_date_arr);
		return (1);
	}
	memset(world->the_end_array, 0, sizeof(int) * num_of_philos);
	return (0);
}


//El argumento philo_n recibe el numero del filosofo conforme esta numerado
//en la mesa, en cambio los filosofos, al estar almacenados en un array
//tienen un indice n-1 de manera que el filosofo 1 ocupa la posicion 0 del array;
int	init_one_philo(t_world *world, unsigned int philo_n)
{
	pthread_t 		*new_philo;
	t_philo_scope	*scope;

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

void	stablish_order_forks(unsigned int n, t_philo_scope *scope, pthread_mutex_t *left,
	pthread_mutex_t *rigth)
{
	if ((n % 2) == 0)
	{
		scope->first_fork = left;
		scope->second_fork = rigth;
	}
	else
	{
		scope->first_fork = rigth;
		scope->second_fork = left;
	}
}

t_philo_scope	*scoop_of_this_philo(t_world *world, unsigned int philo_n)
{
	t_philo_scope	*scope;
	pthread_mutex_t	*left;
	pthread_mutex_t	*rigth;

	scope = malloc(sizeof(t_philo_scope));
	if (scope == NULL)
		return (NULL);
	scope->name = philo_n + 1;
	scope->argx = world->argx;
	left= find_left_fork(world->forks, philo_n, world->argx[NUM_OF_PHILO]);
	rigth = &(world->forks[philo_n]);

	stablish_order_forks(philo_n, scope, left, rigth);

	scope->start_date = world->start_date;
	scope->dead_date = &(world->dead_date_arr[philo_n]);
	scope->dead_date_mutex = &(world->dead_date_mutex_arr[philo_n]);
	scope->the_end = &(world->the_end_array[philo_n]);
	scope->mutex_end = &(world->mutex_end_array[philo_n]);
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
