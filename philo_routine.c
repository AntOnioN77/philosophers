/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 23:05:55 by antofern          #+#    #+#             */
/*   Updated: 2025/05/07 17:17:42 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


void *even_philo(void *sc)
{
	t_philo_scope	*scope;
	int				eats;
    long long       start_time;

    //start
	eats = 0;
	scope = (t_philo_scope *)sc;
    start_time = get_time_ms();
	pthread_mutex_lock(scope->dead_mutex);       //-ML
    *(scope->dead) = start_time + scope->argx[1];//
	pthread_mutex_unlock(scope->dead_mutex);	 //-MU
	monitor(scope,  start_time, "is thinking");

	pthread_mutex_lock(scope->mutex_end);
	while(!scope->the_end)
	{
		pthread_mutex_unlock(scope->mutex_end);

        //take first fork
		ptread_mutex_lock(scope->right_fork);
		if(monitor(scope,  start_time, "has taken a fork"))
		{
			ptread_mutex_unlock(scope->right_fork);
			return (sc);
		}
        //take second fork
		ptread_mutex_lock(scope->left_fork);
		if(monitor(scope,  start_time, "has taken a fork"))
		{
			ptread_mutex_unlock(scope->left_fork);
			ptread_mutex_unlock(scope->right_fork);
			return (sc);
		}
        //and eat
		pthread_mutex_lock(scope->dead_mutex);		 		// -ML
        *(scope->dead) = *(scope->dead) + scope->argx[1];	//
		pthread_mutex_unlock(scope->dead_mutex);      		//-MU
		if(monitor(scope,  start_time, "is eating"))
		{
			ptread_mutex_unlock(scope->left_fork);
			ptread_mutex_unlock(scope->right_fork);
			return (sc);
		}
		eats++;
		usleep(scope->argx[2] * 1000);
		ptread_mutex_unlock(scope->left_fork);
		ptread_mutex_unlock(scope->right_fork);
        //duerme y piensa
		if(monitor(scope,  start_time, "is sleeping"))
			return (sc);
		usleep(scope->argx[3] * 1000);
		if(eats >= scope->argx[4])
			return (sc);
		if(monitor(scope,  start_time, "is thinking"))
			return (sc);
		
		pthread_mutex_lock(scope->mutex_end);
	}
	free(sc);
	return (NULL);
}

//MOOK
void *odd_philo(void *sc)
{
	t_philo_scope	*scope;
	int				eats;
    long long       start_time;

    //start
	eats = 0;
	scope = (t_philo_scope *)sc;
    start_time = get_time_ms();
	pthread_mutex_lock(scope->dead_mutex);       //-ML
    *(scope->dead) = start_time + scope->argx[1];//
	pthread_mutex_unlock(scope->dead_mutex);	 //-MU
	monitor(scope,  start_time, "is thinking");

	pthread_mutex_lock(scope->mutex_end);
	while(!scope->the_end)
	{
		pthread_mutex_unlock(scope->mutex_end);

        //take first fork
		ptread_mutex_lock(scope->left_fork);
		if(monitor(scope,  start_time, "has taken a fork"))
		{
			ptread_mutex_unlock(scope->left_fork);
			return (sc);
		}
        //take second fork
		ptread_mutex_lock(scope->right_fork);
		if(monitor(scope,  start_time, "has taken a fork"))
		{
			ptread_mutex_unlock(scope->left_fork);
			ptread_mutex_unlock(scope->right_fork);
			return (sc);
		}
        //and eat
		pthread_mutex_lock(scope->dead_mutex);		 		// -ML
        *(scope->dead) = *(scope->dead) + scope->argx[1];	//
		pthread_mutex_unlock(scope->dead_mutex);      		//-MU
		if(monitor(scope,  start_time, "is eating"))
		{
			ptread_mutex_unlock(scope->left_fork);
			ptread_mutex_unlock(scope->right_fork);
			return (sc);
		}
		eats++;
		usleep(scope->argx[2] * 1000);
		ptread_mutex_unlock(scope->left_fork);
		ptread_mutex_unlock(scope->right_fork);
        //duerme y piensa
		if(monitor(scope,  start_time, "is sleeping"))
			return (sc);
		usleep(scope->argx[3] * 1000);
		if(eats >= scope->argx[4])
			return (sc);
		if(monitor(scope,  start_time, "is thinking"))
			return (sc);
		
		pthread_mutex_lock(scope->mutex_end);
	}
	free(sc);
	return (NULL);
}


long long	get_time_ms(void)
{
    struct timeval tv;    

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	monitor(t_philo_scope *scp, long long *start, char *msg)
{
	long long		time;
	int				the_end;
	char			*full_msg;

	time = get_time_ms();

	pthread_mutex_lock(scp->mutex_end);
	the_end = scp->the_end;
	pthread_mutex_unlock(scp->mutex_end);

	pthread_mutex_lock(scp->dead_mutex);
	if(time > scp->dead || the_end != 0)// the dead debe ir muteado
		return(1);
	pthread_mutex_unlock(scp->dead_mutex);
	full_msg = cmpmsg(start, time, scp, msg);
	if (!full_msg)
		return (1);
	ft_putstr_fd(full_msg, 1);
	free(full_msg);
	
	return (0);
}


/*Une str_a y str_b creando un nuevo string y lo almacena en result.
Si no pudo crearlo libera str_a y str_b y retorna null*/
char	*join_check(char *str_a, char *str_b, char **result)
{
	*result = ft_strjoin(str_a, str_b);
	if(*result == NULL)
	{
		free(str_a);
		free(str_b);
		return(NULL);
	}
	return(*result);
}

char	*cmpmsg(long long *start, long long time, t_philo_scope *scp, char *msg)
{
	char		*full_msg;
	char		*aux;
	char		*name;

	time = time - *start;
	if(time > INT_MAX || scp->name > INT_MAX);
		return(NULL);
	aux = ft_itoa(time);
	if(aux == NULL)
		return(NULL);
	if(!join_check(aux, " ", full_msg))
		return (NULL);
	free(aux);
	name = ft_itoa(scp->name);
	if(name == NULL)
		return(NULL);
	if(!join_check(full_msg, name, aux))
		return(NULL);
	free(full_msg);
	free(name);
	full_msg = ft_strjoin(aux, msg);
	return(full_msg);
}
