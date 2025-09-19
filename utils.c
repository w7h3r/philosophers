/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:25:01 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/14 19:07:57 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	should_sim_stop(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->death_mutex);
	status = data->life;
	pthread_mutex_unlock(&data->death_mutex);
	return (!status);
}

int	get_first_fork_id(t_philo *philo)
{
	if (philo->left_fork < philo->right_fork)
		return (philo->left_fork);
	else
		return (philo->right_fork);
}

int	get_second_fork_id(t_philo *philo)
{
	if (philo->left_fork > philo->right_fork)
		return (philo->left_fork);
	else
		return (philo->right_fork);
}

long long	get_philo_last_meal_time(t_philo *philo)
{
	long long	last_meal_time;

	pthread_mutex_lock(&philo->last_meal_mutex);
	last_meal_time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->last_meal_mutex);
	return (last_meal_time);
}
