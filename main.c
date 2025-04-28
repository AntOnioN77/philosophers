/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:31:33 by antofern          #+#    #+#             */
/*   Updated: 2025/04/24 14:28:54 by antofern         ###   ########.fr       */
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
	if(init_forks(world))
		return(1); //sale limpio
	if(init_observer(world))
		return(2);//destruir mutex, liberar world->forks, liberar world->mutex
	if (init_philosophers(world))
		return (3);
	return(0);
}

int	free_simulated_world(int sim_ret, t_world *world)
{}

int	init_observer()
{}

int init_philosophers(unsigned int argx[])
{
	unsigned int n_philos;

	n_philos = argx[0];
	while(n_philos > 0)
	{
		//to do
		

		n_philos--;
	}

}

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