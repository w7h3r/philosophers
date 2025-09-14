/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:39:48 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/13 17:32:08 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

int	philo_create_failed(t_data *data, t_philo *philo, int i)
{
	pthread_mutex_lock(&data->death_mutex);
	data->life = 0;
	pthread_mutex_unlock(&data->death_mutex);
	while (i-- > 0)
		pthread_join(philo[i].thread, NULL);
	free_all(data, philo);
	return (1);
}

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

int	init_philos(t_data *data, t_philo **philo)
{
	int	i;

	*philo = malloc(sizeof(t_philo) * data->philosopher_num);
	if (!*philo)
		return (1);
	data->philos = *philo;
	i = 0;
	while (i < data->philosopher_num)
	{
		init_philo_values(data, *philo, i);
		if (pthread_mutex_init(&(*philo)[i].last_meal_mutex, NULL) != 0)
		{
			free_all(data, NULL);
			return (1);
		}
		if (pthread_create(&(*philo)[i].thread,
			NULL, (void *(*)(void *))philo_routine, &(*philo)[i]) != 0)
			return (philo_create_failed(data, *philo, i++));
		i++;
	}
	return (0);
}

int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philosopher_num);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->philosopher_num)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			destroy_forks(data, i);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init(t_data *data, t_philo **philos, int argc, char **argv)
{
	data->life = 0;
	data->philos = NULL;
	data->forks = NULL;
	data->start_time = 0;
	*philos = NULL;
	if (parse_arguments(argc, argv, data) == 1)
		return (free_all_and_return(data, NULL, 1));
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (free_all_and_return(data, NULL, 1));
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
		return (free_all_and_return(data, NULL, 1));
	if (pthread_mutex_init(&data->times_ate_mutex, NULL) != 0)
		return (free_all_and_return(data, NULL, 1));
	if (pthread_mutex_init(&data->all_ate_mutex, NULL) != 0)
		return (free_all_and_return(data, NULL, 1));
	if (init_forks(data) == 1)
		return (free_all_and_return(data, NULL, 1));
	if (init_philos(data, philos) == 1)
		return (free_all_and_return(data, *philos, 1));
	data->all_ate = 0;
	return (0);
}
