/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:11:48 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/13 17:28:43 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_take_first_fork(t_philo *philo)
{
	t_data	*data;
	int		f_fork;

	data = philo->data;
	f_fork = get_first_fork_id(philo);
	if (should_sim_stop(data) == 1)
		return (1);
	pthread_mutex_lock(&data->forks[f_fork]);
	if (should_sim_stop(data) == 1)
	{
		pthread_mutex_unlock(&data->forks[f_fork]);
		return (1);
	}
	print_status(data, philo->id, "has taken a fork");
	if (should_sim_stop(data) == 1)
	{
		pthread_mutex_unlock(&data->forks[f_fork]);
		return (1);
	}
	return (0);
}

int	philo_take_second_fork(t_philo *philo)
{
	t_data	*data;
	int		s_fork;

	data = philo->data;
	s_fork = get_second_fork_id(philo);
	if (should_sim_stop(data) == 1)
		return (1);
	pthread_mutex_lock(&data->forks[s_fork]);
	if (should_sim_stop(data) == 1)
	{
		pthread_mutex_unlock(&data->forks[s_fork]);
		return (1);
	}
	print_status(data, philo->id, "has taken a fork");
	if (should_sim_stop(data) == 1)
	{
		pthread_mutex_unlock(&data->forks[s_fork]);
		return (1);
	}
	return (0);
}

int	philo_take_forks(t_philo *philo)
{
	t_data	*data;

	if (philo_take_first_fork(philo) == 1)
		return (1);
	data = philo->data;
	if (data->philosopher_num == 1)
	{
		pthread_mutex_unlock(&data->forks[get_first_fork_id(philo)]);
		return (1);
	}
	if (philo_take_second_fork(philo) == 1)
	{
		pthread_mutex_unlock(&data->forks[get_first_fork_id(philo)]);
		return (1);
	}
	if (should_sim_stop(data) == 1)
	{
		pthread_mutex_unlock(&data->forks[get_first_fork_id(philo)]);
		pthread_mutex_unlock(&data->forks[get_second_fork_id(philo)]);
		return (1);
	}
	return (0);
}

int	philo_put_down_forks(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_unlock(&data->forks[get_second_fork_id(philo)]);
	pthread_mutex_unlock(&data->forks[get_first_fork_id(philo)]);
	return (0);
}
