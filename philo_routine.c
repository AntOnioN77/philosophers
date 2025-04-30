/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 23:05:55 by antofern          #+#    #+#             */
/*   Updated: 2025/04/30 17:49:18 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


void *even_philo(void *sc)
{
	t_philo_scope	*scope;
	int				eats;
    long long       start_time;

    //start
	eats = 0;
	scope = (t_philo_scope *)sc;
    start_time_count(&start_time);
	pthread_mutex_lock(scope->dead_mutex);       //-ML
    *(scope->dead) = start_time + scope->argx[1];//
	pthread_mutex_unlock(scope->dead_mutex);	 //-MU
	monitor(scope,  start_time, "is thinking");

	pthread_mutex_lock(scope->mutex_end);
	while(!scope->the_end)
	{
		pthread_mutex_unlock(scope->mutex_end);

        //take first fork
		ptread_mutex_lock(scope->right_fork);
		if(monitor(scope,  start_time, "has taken a fork"))
			return (sc);
        //take second fork
		ptread_mutex_lock(scope->left_fork);
		if(monitor(scope,  start_time, "has taken a fork"))
			return (sc);
        //and eat
		pthread_mutex_lock(scope->dead_mutex);		  // -ML
        *(scope->dead) = scope->dead + scope->argx[1];//
		pthread_mutex_unlock(scope->dead_mutex);      //-MU
		if(monitor(scope,  start_time, "is eating"))
			return (sc);
		eats++;
		usleep(scope->argx[2] * 1000);
		ptread_mutex_unlock(scope->left_fork);
		ptread_mutex_unlock(scope->right_fork);
        //duerme y piensa
		if(monitor(scope,  start_time, "is sleeping"))
			return (sc);
		usleep(scope->argx[3] * 1000);
		if(eats >= scope->argx[4])
			return (sc);
		if(monitor(scope,  start_time, "is thinking"))
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

int	monitor(t_philo_scope *scp, long long *start, char *msg)
{
	long long		time;
	int				the_end;
	char			*full_msg;
	struct timeval	tv;


	gettimeofday(&tv, NULL);
	time = tv_miliseconds(start, &tv);
	pthread_mutex_lock(scp->mutex_end);
	the_end = scp->the_end;
	pthread_mutex_unlock(scp->mutex_end);
	if(time > scp->dead || the_end != 0)// the end debe ir muteado
		return(1);
	full_msg = compmsg(start, time, scp, msg);
	if (!full_msg)
		return (1);
	ft_putstr_fd(full_msg, 1);
	free(full_msg);
	
	return (0);
}



long long	tv_miliseconds(long long *start, struct timeval *tv)
{
	long long	miliseconds;
	
	miliseconds = (tv->tv_sec * 1000) + (tv->tv_usec / 1000);
	return (miliseconds);
}

char	*cmpmsg(long long *start, long long time, t_philo_scope *scp, char *msg)
{
	long long	*time;
	char		*aux;
	char		*timestamp;
	char		*name;

	time = time - *start;
	if(time > INT_MAX || scp->name > INT_MAX);
		return(NULL);
	timestamp = ft_itoa(time);
	//checknull
	name = ft_itoa(scp->name);
	//checknull
	aux = ft_strjoin(timestamp, " ");
	//checknull
	free(timestamp);
	ft_strjoin(aux, name);
	////////////////////////// mucha cosa...XD

}