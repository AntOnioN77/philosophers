/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:31:33 by antofern          #+#    #+#             */
/*   Updated: 2025/05/18 12:26:30 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	simulation(t_world *world);

int main(int argc, char **argv)
{
	t_world world;

	memset(&world, 0, sizeof(t_world));
	if(parse_args(argc, argv, world.argx))
		return (1);
	if(free_simulated_world(simulation(&world), &world))
		return (1);
	return (0);
}

//TIME_TO_DIE - (TIME_TO_EAT * 3 + TIME_TO_SLEEP) - (DELAY_FACTOR * NUM_OF_PHILO)
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

// ESTE ALGORITMO ES PARA IMPARES, se puede optimizar para que los pares piensen mas tiempo
	if(argx[NUM_OF_PHILO] % 2 != 0)
	{

printf("main 47 IMPAR numero de philosofos\n");
fflush(NULL);
		if(time_to_sleep < (time_to_eat * 2))
			total = (time_to_die - time_to_eat - ((time_to_eat * 2) - time_to_sleep)) * 1000;
		else
			total = (time_to_die - time_to_eat - time_to_sleep) * 1000;
	}
	else
	{
printf("main 47 PAR numero de philosofos\n");
fflush(NULL);
		total = (time_to_die - time_to_eat - time_to_sleep) * 1000;
	}

	if(total - delay > 0)
		*tinking_time = (unsigned int) (total - delay);
	else
		*tinking_time = 0;	

printf("main l46 tiniking_time:%u\n", *tinking_time);
fflush(NULL);
}

int	simulation(t_world *world)
{
	agree_time_tinking(&(world->tinking_time), world->argx);
//	world->tinking_time = 1000;
	if(create_mutexes(world))
		return(1); //sale limpio
	if (init_philosophers(world))
		return (3);
	if(pthread_create(&(world->observer), NULL, observer_routine, world))
		return(2);
	return(0);
}

void	thread_join_all(pthread_t philosophers[], unsigned int n)
{
	void *tread_ret;

	while (n > 0)
	{
		pthread_join(philosophers[n-1], &tread_ret);
		free(tread_ret); //libera scoope, el hilo main es propietario de la memoria scoope
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
	if(TEST)
	{
		usleep(999999);
		printf("*****simulation_return() = %d******\n", sim_ret);
		fflush(NULL);
	}
	//if(sim_ret = 1)//si falla create_mutexes
		//to do: NADA?
	if(sim_ret == 3)//si falla init_philosophers
		destroy_mutexes(world);
	if(sim_ret == 2)//si falla init_observer
	{
		//liberar philos
		thread_join_all(world->philosophers, world->argx[NUM_OF_PHILO]);
		free(world->philosophers);
		free(world->dead_date_arr);
		free(world->state_array);
		//liberar mutexes
		destroy_mutexes(world);
	}
	if(sim_ret == 0)//ejecucion normal no errores
	{
		//liberar philos y observer
		pthread_join(world->observer, NULL);//el observer debe terminar ¿antes? que los philosofos
		thread_join_all(world->philosophers, world->argx[NUM_OF_PHILO]);

		free(world->philosophers);
		free(world->dead_date_arr);
		free(world->state_array);
		//liberar mutexes
		destroy_mutexes(world);
		return(0);
	}
	return(1);
}


int    parse_args(int argc, char *argv[], unsigned int argx[])
{
	int i;

	if (validate_args(argc, argv))
		return (1);
	i = 0;
	while(i < 4)
	{
		argx[i] = ft_atol(argv[i+1]);
		i++;
	}
	if(argc == 6)
		argx[4] = ft_atol(argv[5]);
	return(0);
}