/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 16:37:02 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/14 19:10:13 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

int	has_everyone_eaten(t_data *data)
{
	int	all_ate;

	if (data->philosopher_must_eat_num == -1)
		return (0);
	pthread_mutex_lock(&data->all_ate_mutex);
	if (data->all_ate >= data->philosopher_num)
		all_ate = 1;
	else
		all_ate = 0;
	pthread_mutex_unlock(&data->all_ate_mutex);
	return (all_ate);
}

void	philo_death(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&data->death_mutex);
	data->life = 0;
	pthread_mutex_unlock(&data->death_mutex);
	print_status(data, philo->id, "died");
}

void	philo_ate_enough(t_data *data)
{
	pthread_mutex_lock(&data->death_mutex);
	data->life = 0;
	pthread_mutex_unlock(&data->death_mutex);
}

void	pre_monitor(t_data *data)
{
	usleep(100);
	data->start_time = get_time();
	data->life = 1;
	pthread_mutex_unlock(&data->death_mutex);
}

void	philo_monitor(t_data *data)
{
	long long	current_time;
	long long	last_meal_time;
	int			i;

	pre_monitor(data);
	while (should_sim_stop(data) == 0)
	{
		i = 0;
		while (i < data->philosopher_num)
		{
			current_time = get_time();
			last_meal_time = get_philo_last_meal_time(&data->philos[i]);
			if (current_time - last_meal_time >= data->time_to_die)
				return (philo_death(data, &data->philos[i]));
			i++;
		}
		if (has_everyone_eaten(data) == 1)
			return (philo_ate_enough(data));
	}
}
