/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:31:33 by antofern          #+#    #+#             */
/*   Updated: 2025/05/20 10:19:43 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int		simulation(t_world *world);
void	agree_time_tinking(unsigned int *tinking_time, unsigned int argx[]);

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
