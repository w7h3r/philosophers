/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:25:01 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/13 17:33:32 by muokcan          ###   ########.fr       */
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
	if (philo->id % 2 == 0)
		return (philo->right_fork);
	else
		return (philo->left_fork);
}

int	get_second_fork_id(t_philo *philo)
{
	if (philo->id % 2 == 0)
		return (philo->left_fork);
	else
		return (philo->right_fork);
}
