/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:31:33 by antofern          #+#    #+#             */
/*   Updated: 2025/05/10 19:11:39 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int     init_philosophers(argx);
int    parse_args(int argc, char *argv[], unsigned int argx[]);

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

int	simulation(t_world *world)
{
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
	n--;
	while (n >= 0)
	{
		pthread_join(philosophers[n], &tread_ret);
		free(tread_ret); //libera scoope, el hilo main es propietario de la memoria scoope
	}
}

void	destroy_mutexes(t_world *world)
{
	destroy_arr_mutex(world->forks, world->argx[NUM_OF_PHILO]);
	free(world->forks);
	destroy_arr_mutex(world->dead_date_mutex_arr, world->argx[NUM_OF_PHILO]);
	free(world->dead_date_mutex_arr);
	pthread_mutex_destroy(&(world->mutex_end));
}

int	free_simulated_world(int sim_ret, t_world *world)
{

	//if(sim_ret = 1)//si falla create_mutexes
		//to do: NADA?
	if(sim_ret = 3)//si falla init_philosophers
		destroy_mutexes(world);
	if(sim_ret = 2)//si falla init_observer
	{
		//liberar philos
		thread_join_all(world->philosophers, world->argx[NUM_OF_PHILO]);
		free(world->philosophers);
		free(world->birth_date_arr);
		//liberar mutexes
		destroy_mutexes(world);
	}
	if(sim_ret = 0)//ejecucion normal no errores
	{
		//liberar philos y observer
		pthread_join(world->observer, NULL);//el observer debe terminar ¿antes? que los philosofos
		thread_join_all(world->philosophers, world->argx[NUM_OF_PHILO]);

		free(world->philosophers);

		free(world->birth_date_arr);
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