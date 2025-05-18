/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 23:05:55 by antofern          #+#    #+#             */
/*   Updated: 2025/05/18 15:00:52 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


t_bool	call_observer(t_states *state, pthread_mutex_t *mutex_state, unsigned int name)
{
name = name + 0;///BORARR
	pthread_mutex_lock(mutex_state);
	if(*state == YES_EAT || *state == THE_END)
	{
/*fprintf(stderr,"----------------%u YES_EAT--%lld\n", name, get_time_ms() - start);
fflush(NULL);*/
		pthread_mutex_unlock(mutex_state);
		return(TRUE);
	}
	else
	{
/*fprintf(stderr,"----------------%u NO_EAT--%lld\n", name, get_time_ms() - start);
fflush(NULL);*/
		pthread_mutex_unlock(mutex_state);
		return (FALSE);
	}
}

void *philo_routine(void *sc)
{
	t_philo_scope	*scope;
	unsigned int	eats;
    long long       start_time;

    //start
	scope = (t_philo_scope *)sc;
	eats = 0;
    start_time = scope->start_date;
	monitor(scope,  start_time, " is thinking\n");
	/*if(scope->name % 2 == 0)
	{
		printf("name:%d\n",scope->name);
		usleep(EVEN_WAIT * scope->argx[NUM_OF_PHILO]);
	}*/
	while(1)
	{
        //take first fork
		if(scope->first_fork == NULL)
			return (sc);
		while(!call_observer(scope->state, scope->mutex_state, scope->name))
		{
	printf("name:%d Is Waiting, state:%d\n",scope->name, *(scope->state));
	fflush(NULL);

			usleep(RECALL_WAIT);
		}
	printf("name:%d Is still loving you, state:%d\n",scope->name, *(scope->state));
	fflush(NULL);
		pthread_mutex_lock(scope->first_fork);
		if(monitor(scope,  start_time, " has taken a fork\n"))
		{
			pthread_mutex_unlock(scope->first_fork);
//printf("monitor fallo en philo_routine (l66)");
//fflush(NULL);
			return (sc);
		}
        //take second fork
		if(scope->second_fork == NULL)
		{
			pthread_mutex_unlock(scope->first_fork);
			return (sc);
		}
		pthread_mutex_lock(scope->second_fork);
		if(monitor(scope,  start_time, " has taken a fork\n"))
		{
			pthread_mutex_unlock(scope->first_fork);
			pthread_mutex_unlock(scope->second_fork);
			return (sc);
		}
        //and eat
		pthread_mutex_lock(scope->dead_date_mutex);		 		// -ML
        *(scope->dead_date) = get_time_ms() + (scope->argx[TIME_TO_DIE]);	//
		pthread_mutex_unlock(scope->dead_date_mutex);      		//-MU
		if(monitor(scope,  start_time, " is eating\n"))
		{
			pthread_mutex_unlock(scope->first_fork);
			pthread_mutex_unlock(scope->second_fork);
			return (sc);
		}
		pthread_mutex_lock(scope->mutex_state);
		*(scope->state) = EATING;
		pthread_mutex_unlock(scope->mutex_state);
		eats++;
		usleep(scope->argx[TIME_TO_EAT] * 1000);
		pthread_mutex_unlock(scope->first_fork);
		pthread_mutex_unlock(scope->second_fork);
		if(scope->argx[MAX_EATS] != 0 && eats >= scope->argx[MAX_EATS])
		{
			pthread_mutex_lock(scope->mutex_state);
			*(scope->state)= THE_END;
			pthread_mutex_unlock(scope->mutex_state);
			return (sc);
		}
        //duerme y piensa
		if(monitor(scope,  start_time, " is sleeping\n"))
			return (sc);
		usleep(scope->argx[TIME_TO_SLEEP] * 1000);

		if(monitor(scope,  start_time, " is thinking\n"))
			return (sc);
		usleep(scope->tinking_time);
	}
	return (sc);
}


long long	get_time_ms(void)
{
    struct timeval tv;    

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	monitor(t_philo_scope *scp, long long start, char *msg)
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
	if(time > *(scp->dead_date))
		good_bye = 1;
	pthread_mutex_unlock(scp->dead_date_mutex);

	if(good_bye)
		return(1);

	full_msg = cmpmsg(start, time, scp->name, msg);
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

char	*cmpmsg(long long start, long long time, unsigned int n, char *msg)
{
	char	*full_msg;
	char	*aux;
	char	*name;

	time = time - start;
	/*if(time > LLONG_MAX || n > _MAX)
		return(NULL);*/
	aux = ft_lltoa(time);
	if(aux == NULL)
		return(NULL);
	if(!join_check(aux, " ", &(full_msg)))
		return (NULL);
	free(aux);
	name = ft_lltoa(n); //estaria bien almacenar el nombre en la funcion llamadora, para no tener que hacer itoa cada vez que halla que imprimir algo
	if(name == NULL)
		return(NULL);
	if(!join_check(full_msg, name, &(aux)))
		return(NULL);
	free(full_msg);
	free(name);
	full_msg = ft_strjoin(aux, msg);
	free(aux);
	return(full_msg);
}
