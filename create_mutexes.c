/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_mutexes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:41:28 by antofern          #+#    #+#             */
/*   Updated: 2025/04/30 17:05:39 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int reserve_memory(t_world *world, int num_of_philos);
static int	init_mutex(t_world *world, int num_of_philos);
static void	panic_destroy_mutex(pthread_mutex_t forks[], int count);
int	init_forks(t_world *world, int num_of_philos);

int	create_mutexes(t_world *world)
{
	int num_of_philos;

	num_of_philos = world->argx[0];
	if(reserve_memory(world, num_of_philos))
		return (1);
	if(init_mutex(world, num_of_philos))
	{
		free(world->forks);
		free(world->dead_mutex);
		return (1);
	}

	return (0);
}

static int reserve_memory(t_world *world, int num_of_philos)
{
	world->forks =  malloc(sizeof(pthread_mutex_t) * num_of_philos);
	if (world->forks ==NULL)
		return (1);
	world->dead_mutex = malloc(sizeof(pthread_mutex_t) * num_of_philos);
	if (world->dead_mutex ==NULL)
	{
		free(world->forks);
		return (1);
	}
	return (0);
}

static int	init_mutex(t_world *world, int num_of_philos)
{
	int	i;

	if(init_forks(world, num_of_philos))
		return (1);
	i = 0;
	while(i < num_of_philos)
	{
		if(pthread_mutex_init(&(world->dead_mutex[i]), NULL))
		{
			panic_destroy_mutex(world->forks, num_of_philos);
			panic_destroy_mutex(world->dead_mutex, i);
			return (1);
		}
		i++;
	}
	if(pthread_mutex_init(&(world->mutex_end), NULL))
		{
			panic_destroy_mutex(world->forks, num_of_philos);
			panic_destroy_mutex(world->dead_mutex, i);
			return (1);
		}
	return (0);
}

int	init_forks(t_world *world, int num_of_philos)
{	
	int	i;

	i = 0;
	while(i < num_of_philos)
	{
		if(pthread_mutex_init(&(world->forks[i]), NULL))
		{
			panic_destroy_mutex(world->forks, i);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	panic_destroy_mutex(pthread_mutex_t forks[], int count)
{
	while(count > 0)
	{
		count--;
		pthread_mutex_destroy(&(forks[count]));
	}	
}