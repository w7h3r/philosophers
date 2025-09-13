/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:26:29 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/13 17:29:01 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	increment_all_ate(t_data *data)
{
	pthread_mutex_lock(&data->all_ate_mutex);
	data->all_ate++;
	pthread_mutex_unlock(&data->all_ate_mutex);
}

int	philo_eat(t_philo *philo)
{
	t_data	*data;
	int		curr_ate_num;

	data = philo->data;
	if (should_sim_stop(data) == 1)
		return (1);
	if (philo_take_forks(philo))
		return (1);
	print_status(data, philo->id, "is eating");
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->last_meal_mutex);
	my_sleep(data->time_to_eat, data);
	pthread_mutex_lock(&data->times_ate_mutex);
	philo->times_ate++;
	curr_ate_num = philo->times_ate;
	pthread_mutex_unlock(&data->times_ate_mutex);
	if (data->philosopher_must_eat_num != -1
		&& curr_ate_num == data->philosopher_must_eat_num)
		increment_all_ate(data);
	philo_put_down_forks(philo);
	return (should_sim_stop(data));
}

int	philo_sleep(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (should_sim_stop(data) == 1)
		return (1);
	print_status(data, philo->id, "is sleeping");
	my_sleep(data->time_to_sleep, data);
	return (should_sim_stop(data));
}

int	philo_think(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (should_sim_stop(data) == 1)
		return (1);
	print_status(data, philo->id, "is thinking");
	my_sleep(data->time_to_think, data);
	return (should_sim_stop(data));
}
