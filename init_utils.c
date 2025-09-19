/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 19:00:05 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/14 19:00:59 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

void	clean_created_philos(t_data *data, t_philo *philo, int count)
{
	int	i;

	data->life = 0;
	pthread_mutex_unlock(&data->death_mutex);
	i = 0;
	while (i < count)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}

void	clean_philo_mutexes(t_philo *philo, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&philo[i].last_meal_mutex);
		i++;
	}
}

void	clean_initd_mutexes(t_data *data)
{
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->times_ate_mutex);
	pthread_mutex_destroy(&data->all_ate_mutex);
}

void	clean_initd(t_data *data)
{
	if (data->forks)
		destroy_forks(data, data->philosopher_num);
	clean_initd_mutexes(data);
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
	if (data->forks)
	{
		free(data->forks);
		data->forks = NULL;
	}
}

int	philo_create_failed(t_data *data, t_philo *philo, int created_philo_count)
{
	clean_created_philos(data, philo, created_philo_count);
	clean_philo_mutexes(philo, created_philo_count);
	free(philo);
	data->philos = NULL;
	clean_initd(data);
	return (1);
}
