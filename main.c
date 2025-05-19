/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:31:33 by antofern          #+#    #+#             */
/*   Updated: 2025/05/19 14:39:47 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	simulation(t_world *world);

int	main(int argc, char **argv)
{
	t_world	world;

	memset(&world, 0, sizeof(t_world));
	if (parse_args(argc, argv, world.argx))
		return (1);
	if (free_simulated_world(simulation(&world), &world))
		return (1);
	return (0);
}

void	agree_time_tinking(unsigned int *tinking_time, unsigned int argx[])
{
	long	delay;
	long	time_to_eat;
	long	time_to_sleep;
	long	time_to_die;
	long	total;

	delay = (long) DELAY_FACTOR * argx[NUM_OF_PHILO];
	time_to_eat = (long) argx[TIME_TO_EAT];
	time_to_sleep = (long) argx[TIME_TO_SLEEP];
	time_to_die = (long) argx[TIME_TO_DIE];
	total = time_to_die - time_to_eat - time_to_sleep;
	if (total - delay > 0)
		*tinking_time = (unsigned int) ((total - delay) * 1000);
	else
		*tinking_time = 0;
}

int	simulation(t_world *world)
{
	agree_time_tinking(&(world->tinking_time), world->argx);
	if (create_mutexes(world))
		return (1);
	if (init_philosophers(world))
		return (3);
	if (pthread_create(&(world->observer), NULL, observer_routine, world))
		return (2);
	return (0);
}

void	thread_join_all(pthread_t philosophers[], unsigned int n)
{
	void	*tread_ret;

	while (n > 0)
	{
		pthread_join(philosophers[n - 1], &tread_ret);
		free(tread_ret);
		n--;
	}
}

void	destroy_mutexes(t_world *world)
{
	destroy_arr_mutex(world->forks, world->argx[NUM_OF_PHILO]);
	free(world->forks);
	destroy_arr_mutex(world->dead_date_mutex_arr, world->argx[NUM_OF_PHILO]);
	free(world->dead_date_mutex_arr);
	destroy_arr_mutex(world->mutex_state_array, world->argx[NUM_OF_PHILO]);
	free(world->mutex_state_array);
}

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


int	parse_args(int argc, char *argv[], unsigned int argx[])
{
	int	i;

	if (validate_args(argc, argv))
		return (1);
	i = 0;
	while (i < 4)
	{
		argx[i] = ft_atol(argv[i+1]);
		i++;
	}
	if (argc == 6)
		argx[4] = ft_atol(argv[5]);
	return (0);
}
