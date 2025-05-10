/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:54:16 by antofern          #+#    #+#             */
/*   Updated: 2025/05/10 18:17:21 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

# include <unistd.h>//YA DEBERIA ESTAR en philosophers.h?!?!?!?

int check_survivors(int n, pthread_mutex_t *dead_date_mutex_arr, long long *dead_date_arr, 	long long *birth_date_arr)
{
    long long   time;
    int         i;
    int         is_dead;

    i = 0;
    is_dead = 0;
    while(i < n)
    {
        time = get_time_ms();
        pthread_mutex_lock(&(dead_date_mutex_arr[i]));
        if (dead_date_arr[i] < time)
        {
            cmpmsg(birth_date_arr, time, 1+1, "died");
            pthread_mutex_lock(&(world->mutex_end));
            world->the_end = 1;
            pthread_mutex_unlock(&(world->mutex_end));
            is_dead = 1;
        }
        pthread_mutex_unlock(&(dead_date_mutex_arr[i]));
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
        if(check_survivors(world))      
            return ;
    }


}
