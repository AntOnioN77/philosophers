/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 23:05:55 by antofern          #+#    #+#             */
/*   Updated: 2025/05/20 09:28:53 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int		take_first_fork(t_scope *scope);
static	t_bool	call_observer(t_states *state, pthread_mutex_t *mutex_state,
					unsigned int name);
static int		take_second_fork(t_scope *scope);
static int		and_eat(t_scope *scope, unsigned int *eats);

void	*philo_routine(void *sc)
{
	t_scope			*scope;
	unsigned int	eats;

	scope = (t_scope *)sc;
	eats = 0;
	monitor(scope, scope->start_date, " is thinking\n");
	while (1)
	{
		if (take_first_fork(scope))
			return (sc);
		if (take_second_fork(scope))
			return (sc);
		if (and_eat(scope, &eats))
			return (sc);
		if (monitor(scope, scope->start_date, " is sleeping\n"))
			return (sc);
		usleep(scope->argx[TIME_TO_SLEEP] * 1000);
		if (monitor(scope, scope->start_date, " is thinking\n"))
			return (sc);
		usleep(scope->tinking_time);
	}
	return (sc);
}

static int	take_first_fork(t_scope *scope)
{
	if (scope->first_fork == NULL)
		return (1);
	while (!call_observer(scope->state, scope->mutex_state, scope->name))
		usleep(RECALL_WAIT);
	pthread_mutex_lock(scope->first_fork);
	if (monitor(scope, scope->start_date, " has taken a fork\n"))
	{
		pthread_mutex_unlock(scope->first_fork);
		return (1);
	}
	return (0);
}

static t_bool	call_observer(t_states *state, pthread_mutex_t *mutex_state,
		unsigned int name)
{
	name = name + 0;
	pthread_mutex_lock(mutex_state);
	if (*state == YES_EAT || *state == THE_END)
	{
		pthread_mutex_unlock(mutex_state);
		return (TRUE);
	}
	else
	{
		pthread_mutex_unlock(mutex_state);
		return (FALSE);
	}
}

static int	take_second_fork(t_scope *scope)
{
	if (scope->second_fork == NULL)
	{
		pthread_mutex_unlock(scope->first_fork);
		return (1);
	}
	pthread_mutex_lock(scope->second_fork);
	if (monitor(scope, scope->start_date, " has taken a fork\n"))
	{
		pthread_mutex_unlock(scope->first_fork);
		pthread_mutex_unlock(scope->second_fork);
		return (1);
	}
	return (0);
}

static int	and_eat(t_scope *scope, unsigned int *eats)
{
	pthread_mutex_lock(scope->dead_date_mutex);
	*(scope->dead_date) = get_time_ms() + (scope->argx[TIME_TO_DIE]);
	pthread_mutex_unlock(scope->dead_date_mutex);
	if (monitor(scope, scope->start_date, " is eating\n"))
	{
		pthread_mutex_unlock(scope->first_fork);
		pthread_mutex_unlock(scope->second_fork);
		return (1);
	}
	pthread_mutex_lock(scope->mutex_state);
	*(scope->state) = EATING;
	pthread_mutex_unlock(scope->mutex_state);
	*eats = *eats + 1;
	usleep(scope->argx[TIME_TO_EAT] * 1000);
	pthread_mutex_unlock(scope->first_fork);
	pthread_mutex_unlock(scope->second_fork);
	if (scope->argx[MAX_EATS] != 0 && *eats >= scope->argx[MAX_EATS])
	{
		pthread_mutex_lock(scope->mutex_state);
		*(scope->state) = THE_END;
		pthread_mutex_unlock(scope->mutex_state);
		return (1);
	}
	return (0);
}
