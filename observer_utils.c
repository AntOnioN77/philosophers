/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:56:48 by antofern          #+#    #+#             */
/*   Updated: 2025/05/20 16:42:01 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	end_all_philos(pthread_mutex_t *mutex_state_array,
	t_states *state_array, unsigned int num_of_philos)
{
	unsigned int	i;

	i = 0;
	while (i < num_of_philos)
	{
		pthread_mutex_lock(&(mutex_state_array[i]));
		state_array[i] = THE_END;
		pthread_mutex_unlock(&(mutex_state_array[i]));
		i++;
	}
}

void	establish_order(int *arr, int prev, int this, int next)
{
	int	i;
	int	j;
	int	tmp;

	i = 0;
	arr[0] = prev;
	arr[1] = this;
	arr[2] = next;
	while (i < 2)
	{
		j = i + 1;
		while (j < 3)
		{
			if (arr[i] > arr[j])
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}
