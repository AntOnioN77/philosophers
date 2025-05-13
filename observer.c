/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:54:16 by antofern          #+#    #+#             */
/*   Updated: 2025/05/13 10:05:26 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

# include <unistd.h>//YA DEBERIA ESTAR en philosophers.h?!?!?!?


int check_survivors(t_world *world, long long *dead_date_arr)
{
	long long       time;
	unsigned int    i;
	unsigned int    ended;
	int				dead;
	unsigned int    n;
	char			*msg;

	n = world->argx[NUM_OF_PHILO];
	i = 0;
	ended = 0;
	dead = 0;
	while(i < n)
	{
		time = get_time_ms();
///////////////////////////////////////////////////////////////////////////////////////		
		if(TEST)
		{
			printf("*****(check_survivors,observer)chekeando philo %d******\n", i+1);
			printf("dead_date=%lld time=%lld\n", dead_date_arr[i], time);
			fflush(NULL);
		}
////////////////////////////////////////////////////////////////////////////////////////
		
		pthread_mutex_lock(&(world->mutex_end_array[i]));
		if(!world->the_end_array[i])
		{
			pthread_mutex_lock(&(world->dead_date_mutex_arr[i]));
			if ( dead_date_arr[i] < time)
			{
				msg = cmpmsg(world->start_date, time, 1+1, " died\n");
				if (msg == NULL)
					return (ERROR);//no se estan desmuteando en caso de error dead_date_mutex_arr ni mutex_end_array 
				ft_putstr_fd(msg, 1);
				//pthread_mutex_lock(&(world->mutex_end_array[i]));				
				world->the_end_array[i] = 1;
				//pthread_mutex_unlock(&(world->mutex_end_array[i]));
				dead++;
			}
			pthread_mutex_unlock(&(world->dead_date_mutex_arr[i]));
		}
		else
			ended++;
		pthread_mutex_unlock(&(world->mutex_end_array[i]));
		i++;

	}
	if (dead || ended == n)
		return (1);
	return(0);
}



void    *observer_routine(void *world)
{
	t_world *wo;

	wo = world;
	usleep(wo->argx[TIME_TO_DIE] / 2);//necesario?
	while(1)
	{

		if(check_survivors(wo, wo->dead_date_arr))      
			return (NULL);
		usleep(1000);
	}
	return(NULL);

}
