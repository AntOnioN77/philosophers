/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:54:16 by antofern          #+#    #+#             */
/*   Updated: 2025/05/17 17:34:19 by antofern         ###   ########.fr       */
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

void stablish_order(int *arr, int prev, int this, int next)
{
	arr[0] = prev;
	arr[1] = this;
	arr[2] = next;
	int i = 0;
	while (i < 2) {
		int j = i + 1;
		while (j < 3) {
			if (arr[i] > arr[j]) {
				int tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	neighborhood_update(pthread_mutex_t *mutex_state_array, t_states *state_array,
	unsigned int this, unsigned int num_of_philo)
{
	int	prev;
	int	next;
	int idx [3];

	//pthread_mutex_lock(&(mutex_state_array[this]));
	//if(state_array[this] == EATING)
	//{
	prev = ((this + num_of_philo) - 1) % num_of_philo;
	next = (this + 1) % num_of_philo;
	//printf("prev:%d, this:%d, next:%d\n", prev, this, next);

	stablish_order(idx,prev, this, next);

/*printf("idx[0]:%d idx[1]:%d idx[2]:%d\n", idx[0], idx[1],idx[2]);
fflush(NULL);*/
	if (idx[0] != idx[1] && idx[0] != idx[2])
		pthread_mutex_lock(&(mutex_state_array[idx[0]])); //para que con num_of_philo = 2 no se cierre dos veces el mismo mutex
	if (idx[1] != idx[2])
		pthread_mutex_lock(&(mutex_state_array[idx[1]])); //para que con num_of_philo = 2 no se cierre dos veces el mismo mutex
	pthread_mutex_lock(&(mutex_state_array[idx[2]]));
	if(state_array[this] == EATING)
	{
		//prevenimos que coma dos veces seguidas retirando el permiso.
		if (state_array[this] == EATING)
			state_array[this] = NO_EAT;
		//damos permiso al philosofo a su derecha
		if(state_array[prev] == NO_EAT)
			state_array[prev] = ALMOST_EAT;
		else if (state_array[prev] == ALMOST_EAT)
			state_array[prev] = YES_EAT;
		//damos permiso al philosofo a su izquierda
		
		if(state_array[next] == NO_EAT)
			state_array[next] = ALMOST_EAT;
		else if (state_array[next] == ALMOST_EAT)
			state_array[next] = YES_EAT;
	}
	pthread_mutex_unlock(&(mutex_state_array[idx[2]]));
	if (idx[1] != idx[2])
		pthread_mutex_unlock(&(mutex_state_array[idx[1]]));
	if (idx[0] != idx[1] && idx[0] != idx[2])
		pthread_mutex_unlock(&(mutex_state_array[idx[0]]));
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
//			if (world->state_array[i] == EATING)
//			{
//				neighborhood_update(world->mutex_state_array, world->state_array, i, n);
//			}
		}
		else
			ended++;
		pthread_mutex_unlock(&(world->mutex_state_array[i]));
		neighborhood_update(world->mutex_state_array, world->state_array, i, n);
		
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
