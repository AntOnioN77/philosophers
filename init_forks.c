/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:41:28 by antofern          #+#    #+#             */
/*   Updated: 2025/04/24 14:20:08 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int reserve_memory(t_world *world, int num_of_forks);
static int	init_mutex(t_world *world, int num_of_forks);
static void	panic_destroy_mutex(pthread_mutex_t mute_arr[], int count);

int	init_forks(t_world *world)
{
	int num_of_forks;

	num_of_forks = world->argx[0];
	if(reserve_memory(world, num_of_forks))
		return (1);
	if(init_mutex(world, num_of_forks))
	{
		free(world->forks);
		free(world->mute_arr);
		return (1);
	}

	return (0);
}

static int reserve_memory(t_world *world, int num_of_forks)
{
	world->forks = malloc(sizeof(int) * num_of_forks);
	if (world->forks == NULL)
		return (1);
	memset(world->forks, 0, sizeof(int) * num_of_forks);
	world->mute_arr =  malloc(sizeof(pthread_mutex_t) * num_of_forks);
	if (world->mute_arr ==NULL)
	{
		free(world->forks);
		return (1);
	}
	return (0);
}

static int	init_mutex(t_world *world, int num_of_forks)
{
	pthread_mutex_t *mute_arr;
	int	i;

	mute_arr = world->mute_arr;
	i = 0;
	while(i < num_of_forks)
	{
		if(pthread_mutex_init(&(mute_arr[i]), NULL))
		{
			panic_destroy_mutex(mute_arr, i);
			return (1);
		}
		i++;
	}
	return (0);
}
static void	panic_destroy_mutex(pthread_mutex_t mute_arr[], int count)
{
	while(count > 0)
	{
		pthread_mutex_destroy(&(mute_arr[count -1]));
		count--;
	}	
}