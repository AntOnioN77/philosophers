/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 23:05:55 by antofern          #+#    #+#             */
/*   Updated: 2025/04/30 16:29:23 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


void *even_philo(void *sc)
{
	t_philo_scope	*scope;
	int				eats;
    long long       start_time;
	struct timeval	tv;

    //start
	eats = 0;
	scope = (t_philo_scope *)sc;
    start_time_count(&start_time);
	pthread_mutex_lock(scope->dead_mutex);       //-ML
    *(scope->dead) = start_time + scope->argx[1];//
	pthread_mutex_unlock(scope->dead_mutex);	 //-MU
	monitor(scope,  start_time, &tv, "is thinking");

	pthread_mutex_lock(scope->mutex_end);
	while(!scope->the_end)
	{
		pthread_mutex_unlock(scope->mutex_end);

        //take first fork
		ptread_mutex_lock(scope->right_fork);
		gettimeofday(&tv, NULL);
		if(monitor(scope,  start_time, &tv, "has taken a fork"))
			return (sc);
        //take second fork
		ptread_mutex_lock(scope->left_fork);
		gettimeofday(&tv, NULL);
		if(monitor(scope,  start_time, &tv, "has taken a fork"))
			return (sc);
        //and eat
		pthread_mutex_lock(scope->dead_mutex);		  // -ML
        *(scope->dead) = scope->dead + scope->argx[1];//
		pthread_mutex_unlock(scope->dead_mutex);      //-MU
		if(monitor(scope,  start_time, &tv, "is eating"))
			return (sc);
		eats++;
		usleep(scope->argx[2] * 1000);
		ptread_mutex_unlock(scope->left_fork);
		ptread_mutex_unlock(scope->right_fork);
        //duerme y piensa
		if(monitor(scope,  start_time, &tv, "is sleeping"))
			return (sc);
		usleep(scope->argx[3] * 1000);
		if(eats >= scope->argx[4])
			return (sc);
		if(monitor(scope,  start_time, &tv, "is thinking"))
			return (sc);
		
		pthread_mutex_lock(scope->mutex_end);
	}
}

void    start_time_count(long long *start_time)
{
    struct timeval tv;    

    gettimeofday(&tv, NULL);
    *start_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

int	monitor(t_philo_scope *scp, long long *start, struct timeval *tv, char *msg)
{
	char	*time;
	int		the_end;

	time = tv_miliseconds_to_char(start, tv);
	if(!time)
		return(1);
	pthread_mutex_lock(scp->mutex_end);
	the_end = scp->the_end;
	pthread_mutex_unlock(scp->mutex_end);
	if(time > scp->dead || the_end != 0)// the end debe ir muteado
		return(1);
	return (0);
}

char	*tv_miliseconds_to_char(long long *start, struct timeval *tv)
{
	long long	miliseconds;
	
	miliseconds = (tv->tv_sec * 1000) + (tv->tv_usec / 1000) - *start;
	if(miliseconds > INT_MAX)
		return(NULL);
	return (ft_itoa((int) miliseconds));
}

