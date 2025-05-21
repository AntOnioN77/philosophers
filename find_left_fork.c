/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_left_fork.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 13:22:09 by antofern          #+#    #+#             */
/*   Updated: 2025/05/21 13:23:05 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

pthread_mutex_t	*find_left_fork(pthread_mutex_t *forks, int philo_n,
				unsigned int total_philo)
{
	if (total_philo == 1)
		return (NULL);
	if (philo_n == 0)
		return (&(forks[total_philo -1]));
	else
		return (&(forks[philo_n -1]));
}
