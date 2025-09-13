/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:21:26 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/13 17:21:46 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void	print_status(t_data *data, int id, char *status)
{
	int	current_life_status;

	pthread_mutex_lock(&data->print_mutex);
	pthread_mutex_lock(&data->death_mutex);
	current_life_status = data->life;
	pthread_mutex_unlock(&data->death_mutex);
	if (!current_life_status && status[0] != 'd')
	{
		pthread_mutex_unlock(&data->print_mutex);
		return ;
	}
	printf("%lld %d %s\n", get_time() - data->start_time, id, status);
	pthread_mutex_unlock(&data->print_mutex);
}
