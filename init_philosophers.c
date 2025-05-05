/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philosophers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:49:28 by antofern          #+#    #+#             */
/*   Updated: 2025/05/05 17:19:13 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_one_philo(t_world *world, unsigned int philo_n, int type);
static int reserve_memory(t_world *world, int num_of_philos);

int init_philosophers(t_world *world)
{
	unsigned int	n_philos;
	unsigned int	current;
	int				error;

	n_philos = world->argx[0];
	if(reserve_memory(world, n_philos))
		return (1);
	current = 1;
	error = 0;
	while(current <= n_philos)
	{
		if((current % 2) == 0)
			error = init_one_philo(world, current, EVEN);
		else
			error = init_one_philo(world, current, ODD);
		current++;
		if(error)
		{
			world->the_end = 1;
			return(error);
		}
	}
	return (0);
}

static int reserve_memory(t_world *world, int num_of_philos)
{
	world->philosophers =  malloc(sizeof(pthread_t) * num_of_philos);
	if (world->philosophers ==NULL)
		return (1);
	world->dead_arr =  malloc(sizeof(int) * num_of_philos);
	if (world->dead_arr == NULL)
	{
		free(world->philosophers);
		return (1);
	}
	return (0);
}


//El argumento philo_n recibe el numero del filosofo conforme esta numerado
//en la mesa, en cambio los filosofos, al estar almacenados en un array
//tienen un indice n-1 de manera que el filosofo 1 ocupa la posicion 0 del array;
int	init_one_philo(t_world *world, unsigned int philo_n, int type)
{
	pthread_t 		*new_philo;
	t_philo_scope	*scope;

	philo_n--;

	scope = scoop_of_this_philo(world, philo_n);
	if( scope == NULL)
		return (1);// 
	new_philo = &(world->philosophers[philo_n]);
	if(type == EVEN)
	{	
		if(pthread_create(new_philo, NULL, even_philo, scope))
			return (1);
	}
	else
	{
		if(pthread_create(new_philo, NULL, odd_philo, scope))
			return (1);
	}
	return (0);

}

t_philo_scope	*scoop_of_this_philo(t_world *world, int philo_n)
{
	t_philo_scope	*scope;

	scope = malloc(sizeof(t_philo_scope));
	if (scope == NULL)
		return (NULL);
	scope->name = philo_n + 1;
	scope->argx = world->argx;
	scope->left_fork = find_left_fork(world->forks, philo_n, world->argx[0]);
	scope->right_fork = &(world->forks[philo_n]);
	scope->dead = world->dead_arr[philo_n];
	scope->dead_mutex = &(world->dead_mutex_arr[philo_n]);
	scope->the_end = world->the_end;
	scope->mutex_end = &(world->mutex_end);
	return (scope);
}

pthread_t	*find_left_fork(pthread_t *forks, int philo_n, int total_philo)
{
	if(total_philo == 1)
		return (NULL);
	if(philo_n == 0)
		return (&(forks[total_philo -1]));
	else
		return (&(forks[philo_n -1]));
}
