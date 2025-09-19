/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:34:04 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/13 17:08:47 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

void	free_all(t_data *data)
{
	if (data)
		free_mutexes(data);
	if (data->philos)
	{
		free(data->philos);
	}
}

void	destroy_forks(t_data *data, int remaining_forks)
{
	int	i;

	i = remaining_forks;
	while (i-- > 0)
		pthread_mutex_destroy(&data->forks[i]);
	free(data->forks);
	data->forks = NULL;
}

void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philosopher_num)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

void	free_mutexes(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->philosopher_num)
			pthread_mutex_destroy(&data->forks[i++]);
		free(data->forks);
		data->forks = NULL;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	if (data->philos)
	{
		i = 0;
		while (i < data->philosopher_num)
			pthread_mutex_destroy(&data->philos[i++].last_meal_mutex);
	}
	pthread_mutex_destroy(&data->times_ate_mutex);
	pthread_mutex_destroy(&data->all_ate_mutex);
}
