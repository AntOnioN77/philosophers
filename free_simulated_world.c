/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_simulated_world.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:20:15 by antofern          #+#    #+#             */
/*   Updated: 2025/05/20 10:22:49 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	destroy_mutexes(t_world *world);
static void	thread_join_all(pthread_t philosophers[], unsigned int n);

int	free_simulated_world(int sim_ret, t_world *world)
{
	if (sim_ret == 3)
		destroy_mutexes(world);
	if (sim_ret == 2)
	{
		thread_join_all(world->philosophers, world->argx[NUM_OF_PHILO]);
		free(world->philosophers);
		free(world->dead_date_arr);
		free(world->state_array);
		destroy_mutexes(world);
	}
	if (sim_ret == 0)
	{
		pthread_join(world->observer, NULL);
		thread_join_all(world->philosophers, world->argx[NUM_OF_PHILO]);

		free(world->philosophers);
		free(world->dead_date_arr);
		free(world->state_array);
		destroy_mutexes(world);
		return (0);
	}
	return (1);
}

static void	thread_join_all(pthread_t philosophers[], unsigned int n)
{
	void	*tread_ret;

	while (n > 0)
	{
		pthread_join(philosophers[n - 1], &tread_ret);
		free(tread_ret);
		n--;
	}
}

static void	destroy_mutexes(t_world *world)
{
	destroy_arr_mutex(world->forks, world->argx[NUM_OF_PHILO]);
	free(world->forks);
	destroy_arr_mutex(world->dead_date_mutex_arr, world->argx[NUM_OF_PHILO]);
	free(world->dead_date_mutex_arr);
	destroy_arr_mutex(world->mutex_state_array, world->argx[NUM_OF_PHILO]);
	free(world->mutex_state_array);
}
