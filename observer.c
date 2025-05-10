/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:54:16 by antofern          #+#    #+#             */
/*   Updated: 2025/05/10 19:46:09 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

# include <unistd.h>//YA DEBERIA ESTAR en philosophers.h?!?!?!?

int check_survivors(t_world *world, long long *dead_date_arr, long long *birth_date_arr)
{
    long long       time;
    unsigned int    i;
    int             is_dead;
    unsigned int    n;

    n = world->argx[NUM_OF_PHILO];
    i = 0;
    is_dead = 0;
    while(i < n)
    {
        time = get_time_ms();
        pthread_mutex_lock(&(world->dead_date_mutex_arr[i]));
        if (dead_date_arr[i] < time)
        {
            cmpmsg(birth_date_arr, time, 1+1, "died");
            pthread_mutex_lock(&(world->mutex_end));
            world->the_end = 1;
            pthread_mutex_unlock(&(world->mutex_end));
            is_dead = 1;
        }
        pthread_mutex_unlock(&(world->dead_date_mutex_arr[i]));
        i++;
    }
    if (is_dead)
        return (1);
    return(0);
}

void    *observer_routine(void *world)
{
    t_world *wo;

    wo = world;
    usleep(wo->argx[TIME_TO_DIE] * 500);
    while(1)
    {
        if(check_survivors(wo, wo->dead_date_arr, wo->birth_date_arr))      
            return (NULL);
    }
    return(NULL);

}
