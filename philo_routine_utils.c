/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:23:45 by antofern          #+#    #+#             */
/*   Updated: 2025/05/21 13:28:26 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	monitor(t_scope *scp, long long start, char *msg)
{
	long long		time;
	int				good_bye;
	char			*full_msg;

	time = get_time_ms();
	good_bye = 0;
	pthread_mutex_lock(scp->mutex_state);
	if (*(scp->state) == THE_END)
		good_bye = 1;
	pthread_mutex_unlock(scp->mutex_state);
	pthread_mutex_lock(scp->dead_date_mutex);
	if (time > *(scp->dead_date))
		good_bye = 1;
	pthread_mutex_unlock(scp->dead_date_mutex);
	if (good_bye)
		return (1);
	full_msg = cmpmsg(start, time, scp->name, msg);
	if (!full_msg)
		return (1);
	ft_putstr_fd(full_msg, 1);
	free(full_msg);
	return (0);
}

char	*join_check(char *str_a, char *str_b, char **result)
{
	*result = ft_strjoin(str_a, str_b);
	if (*result == NULL)
	{
		free(str_a);
		free(str_b);
		return (NULL);
	}
	return (*result);
}

char	*cmpmsg(long long start, long long time, unsigned int n, char *msg)
{
	char	*full_msg;
	char	*aux;
	char	*name;

	time = time - start;
	aux = ft_lltoa(time);
	if (aux == NULL)
		return (NULL);
	full_msg = ft_strjoin(aux, " ");
	if (!full_msg)
	{
		free(aux);
		return (NULL);
	}
	free(aux);
	name = ft_lltoa(n);
	if (name == NULL)
		return (NULL);
	if (!join_check(full_msg, name, &(aux)))
		return (NULL);
	free(full_msg);
	free(name);
	full_msg = ft_strjoin(aux, msg);
	free(aux);
	return (full_msg);
}
