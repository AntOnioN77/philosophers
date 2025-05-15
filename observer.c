/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:54:16 by antofern          #+#    #+#             */
/*   Updated: 2025/05/15 22:13:45 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

# include <unistd.h>//YA DEBERIA ESTAR en philosophers.h?!?!?!?

void	end_all_philos(pthread_mutex_t *mutex_state_array,
	t_states *state_array, unsigned int num_of_philos)
{
	unsigned int i;

	i = 0;
	while(i < num_of_philos)
	{
		pthread_mutex_lock(&(mutex_state_array[i]));
		state_array[i] = THE_END;
		pthread_mutex_unlock(&(mutex_state_array[i]));
		i++;
	}
}

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
		
		pthread_mutex_lock(&(world->mutex_state_array[i]));
		if(world->state_array[i] !=THE_END)
		{
			pthread_mutex_lock(&(world->dead_date_mutex_arr[i]));
			if ( dead_date_arr[i] < time)
			{
				msg = cmpmsg(world->start_date, time, i+1, " died\n");
				if (msg == NULL)
					return (ERROR);//no se estan desmuteando en caso de error dead_date_mutex_arr ni mutex_state_array 
				ft_putstr_fd(msg, 1);
				free(msg);
				//pthread_mutex_lock(&(world->mutex_state_array[i]));				
				//world->state_array[i] = 1;
				//pthread_mutex_unlock(&(world->mutex_state_array[i]));
				//TO DO:
				dead++;
			}
			pthread_mutex_unlock(&(world->dead_date_mutex_arr[i]));
			if (world->state_array[i] == EATING)
			{
				/////////////////TO DO///////////////////////////
				//Consulta el estado de el filosofo a la izquiera y a la derecha si es NO_EAT lo cambia a YES_EAT
				// para ello usa mutex por supuesto
			}
		}
		else
			ended++;
		pthread_mutex_unlock(&(world->mutex_state_array[i]));
		i++;
	}
	if (dead || ended == n)
	{
		end_all_philos(world->mutex_state_array,
			world->state_array, world->argx[NUM_OF_PHILO]);
		return (1);
	}
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
		usleep(OBSERVER_PAUSE);//PRUEBAS SOLO?
	}
	return(NULL);

}
