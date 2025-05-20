/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:54:16 by antofern          #+#    #+#             */
/*   Updated: 2025/05/20 10:05:32 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <unistd.h>

static int	check_survivors(t_world *world, long long *dead_date_arr);
static int	is_dead(t_world *world, long long *dead_date_arr, unsigned int i);
static void	neighborhood_update(pthread_mutex_t *mutex_state_array,
				t_states *state_array, unsigned int this,
				unsigned int num_of_philo);
static void	change_states(t_states *state_array,
				unsigned int prev, unsigned int this, unsigned int next);

void	*observer_routine(void *world)
{
	t_world	*wo;

	wo = world;
	usleep(wo->argx[TIME_TO_DIE] / 2);
	while (1)
	{
		if (check_survivors(wo, wo->dead_date_arr))
			return (NULL);
		usleep(OBSERVER_PAUSE);
	}
	return (NULL);
}

static int	check_survivors(t_world *world, long long *dead_date_arr)
{
	unsigned int	i;
	unsigned int	ended;
	int				dead;

	i = 0;
	ended = 0;
	dead = 0;
	while (i < world->argx[NUM_OF_PHILO])
	{
		pthread_mutex_lock(&(world->mutex_state_array[i]));
		if (world->state_array[i] !=THE_END)
			dead += is_dead(world, dead_date_arr, i);
		else
			ended++;
		pthread_mutex_unlock(&(world->mutex_state_array[i]));
		neighborhood_update(world->mutex_state_array, world->state_array, i,
			world->argx[NUM_OF_PHILO]);
		i++;
	}
	if (ended == world->argx[NUM_OF_PHILO])
		return (1);
	if (dead)
		end_all_philos(world->mutex_state_array,
			world->state_array, world->argx[NUM_OF_PHILO]);
	return (0);
}

static int	is_dead(t_world *world, long long *dead_date_arr, unsigned int i)
{
	char		*msg;
	long long	time;

	time = get_time_ms();
	pthread_mutex_lock(&(world->dead_date_mutex_arr[i]));
	if (dead_date_arr[i] < time)
	{
		world->state_array[i] = THE_END;
		msg = cmpmsg(world->start_date, time, i + 1, " died\n");
		if (msg == NULL)
		{
			pthread_mutex_unlock(&(world->dead_date_mutex_arr[i]));
			pthread_mutex_unlock(&(world->mutex_state_array[i]));
			return (1);
		}
		ft_putstr_fd(msg, 1);
		free(msg);
		pthread_mutex_unlock(&(world->dead_date_mutex_arr[i]));
		return (1);
	}
	pthread_mutex_unlock(&(world->dead_date_mutex_arr[i]));
	return (0);
}

static void	neighborhood_update(pthread_mutex_t *mutex_state_array,
	t_states *state_array, unsigned int this, unsigned int num_of_philo)
{
	int	prev;
	int	next;
	int	idx [3];

	prev = ((this + num_of_philo) - 1) % num_of_philo;
	next = (this + 1) % num_of_philo;
	stablish_order(idx, prev, this, next);
	if (idx[0] != idx[1] && idx[0] != idx[2])
		pthread_mutex_lock(&(mutex_state_array[idx[0]]));
	if (idx[1] != idx[2])
		pthread_mutex_lock(&(mutex_state_array[idx[1]]));
	pthread_mutex_lock(&(mutex_state_array[idx[2]]));
	change_states(state_array, prev, this, next);
	pthread_mutex_unlock(&(mutex_state_array[idx[2]]));
	if (idx[1] != idx[2])
		pthread_mutex_unlock(&(mutex_state_array[idx[1]]));
	if (idx[0] != idx[1] && idx[0] != idx[2])
		pthread_mutex_unlock(&(mutex_state_array[idx[0]]));
}

static void	change_states(t_states *state_array,
	unsigned int prev, unsigned int this, unsigned int next)
{
	if (state_array[this] == EATING)
	{
		if (state_array[this] == EATING)
			state_array[this] = NO_EAT;
		if (state_array[prev] == NO_EAT)
			state_array[prev] = ALMOST_EAT;
		else if (state_array[prev] == ALMOST_EAT)
			state_array[prev] = YES_EAT;
		if (state_array[next] == NO_EAT)
			state_array[next] = ALMOST_EAT;
		else if (state_array[next] == ALMOST_EAT)
			state_array[next] = YES_EAT;
	}
}
