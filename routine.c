/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:06:52 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/13 17:07:31 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_life_status(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->death_mutex);
	status = data->life;
	pthread_mutex_unlock(&data->death_mutex);
	return (status);
}

int	philo_live(t_philo *philo, t_data *data)
{
	int		curr_ate_num;

	if (philo_eat(philo) == 1)
		return (1);
	if (data->philosopher_must_eat_num != -1)
	{
		pthread_mutex_lock(&data->times_ate_mutex);
		curr_ate_num = philo->times_ate;
		pthread_mutex_unlock(&data->times_ate_mutex);
		if (curr_ate_num >= data->philosopher_must_eat_num)
			return (1);
	}
	if (philo_sleep(philo) == 1)
		return (1);
	if (philo_think(philo) == 1)
		return (1);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo *)arg;
	data = philo->data;
	pthread_mutex_lock(&data->death_mutex);
	if (data->life == 0)
	{
		pthread_mutex_unlock(&data->death_mutex);
		return (NULL);
	}
	pthread_mutex_unlock(&data->death_mutex);
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->last_meal_mutex);
	if (philo->id % 2 == 0)
		my_sleep(data->time_to_eat / 2, data);
	if (philo->id == data->philosopher_num
		&& data->philosopher_num % 2 == 1 && data->philosopher_num != 1)
		my_sleep(data->time_to_eat / 2, data);
	while (should_sim_stop(data) == 0)
		if (philo_live(philo, data) == 1)
			break ;
	return (NULL);
}
