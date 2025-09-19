/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:39:48 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/14 19:06:10 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdlib.h>

void	init_philo_values(t_data *data, t_philo *philo, int i)
{
	philo[i].id = i + 1;
	philo[i].times_ate = 0;
	philo[i].left_fork = i;
	if (data->philosopher_num == 1)
		philo[i].right_fork = i;
	else
		philo[i].right_fork = (i + 1) % data->philosopher_num;
	philo[i].data = data;
	philo[i].last_meal_time = get_time();
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philosopher_num)
	{
		init_philo_values(data, data->philos, i);
		if (pthread_create(&data->philos[i].thread,
				NULL, philo_routine, &data->philos[i]) != 0)
		{
			pthread_mutex_destroy(&data->philos[i].last_meal_mutex);
			philo_create_failed(data, data->philos, i);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philosopher_num);
	data->philos = malloc(sizeof(t_philo) * data->philosopher_num);
	if (!data->philos || !data->forks)
		return (1);
	i = 0;
	while (i < data->philosopher_num)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			destroy_forks(data, i);
			clean_philo_mutexes(data->philos, i - 1);
			return (1);
		}
		if (pthread_mutex_init(&(data->philos[i].last_meal_mutex), NULL) != 0)
		{
			destroy_forks(data, i + 1);
			clean_philo_mutexes(data->philos, i);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_data_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&data->death_mutex, NULL))
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (1);
	}
	if (pthread_mutex_init(&data->times_ate_mutex, NULL))
	{
		pthread_mutex_destroy(&data->print_mutex);
		pthread_mutex_destroy(&data->death_mutex);
		return (1);
	}
	if (pthread_mutex_init(&data->all_ate_mutex, NULL))
	{
		clean_initd_mutexes(data);
		return (1);
	}
	return (0);
}

int	init(t_data *data, int argc, char **argv)
{
	data->life = 0;
	data->philos = NULL;
	data->forks = NULL;
	data->start_time = 0;
	data->philos = NULL;
	data->all_ate = 0;
	if (parse_arguments(argc, argv, data) == 1)
		return (1);
	if (init_data_mutexes(data))
		return (1);
	if (init_forks(data) == 1)
	{
		clean_initd(data);
		return (1);
	}
	pthread_mutex_lock(&data->death_mutex);
	if (init_philos(data) == 1)
	{
		clean_initd(data);
		return (1);
	}
	return (0);
}
