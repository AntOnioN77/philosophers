/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:31:33 by antofern          #+#    #+#             */
/*   Updated: 2025/05/07 17:45:40 by antofern         ###   ########.fr       */
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
	if(init_observer(world))
		return(2);//destruir mutex, liberar world->forks,
	return(0);
}

void	thread_join_all(pthread_t philosophers[], int n)
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
	destroy_arr_mutex(world->forks, world->argx[0]);
	free(world->forks);
	destroy_arr_mutex(world->dead_mutex_arr, world->argx[0]);
	free(world->dead_mutex_arr);
	pthread_mutex_destroy(&(world->mutex_end));
}

int	free_simulated_world(int sim_ret, t_world *world)
{
	if(1)//si falla create_mutexes
	//to do: NADA?
	if(3)//si falla init_philosophers
		destroy_mutexes(world);
	if(2)//si falla init_observer
	{
		//liberar philos
		thread_join_all(world->philosophers, world->argx[0]);
		free(world->philosophers);
		//liberar mutexes
		destroy_mutexes(world);
	}
	if(0)//ejecucion normal no errores
	{
		//liberar philos y observer
		thread_join_all(world->philosophers, world->argx[0]);
		pthread_join(world->observer, NULL);
		free(world->philosophers);
		//liberar mutexes
		destroy_mutexes(world);
		return(0);
	}
	return(1);
}

int	init_observer()
{}

int    parse_args(int argc, char *argv[], unsigned int argx[])
{
	int i;

	if (validate_args(argc, argv))
	return (1);
	i = 0;
	while(i < 5)
	{
		argx[i] = ft_atol(argv[i+1]);
		i++;
	}
	return(0);
}