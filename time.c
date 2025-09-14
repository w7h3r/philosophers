/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:09:04 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/13 17:25:28 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <unistd.h>

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long long	set_think_time(t_data *data)
{
	long long	think_time;

	think_time = data->time_to_die - (data->time_to_sleep + data->time_to_eat);
	if (think_time <= 5)
		return (0);
	if (data->philosopher_num > 100)
		return (think_time / 10);
	else if (data->philosopher_num > 50)
		return (think_time / 6);
	else
		return (think_time / 3);
}

void	my_sleep(long long time_in_ms, t_data *data)
{
	long long	start_time;

	start_time = get_time();
	while (get_time() - start_time < time_in_ms)
	{
		if (should_sim_stop(data) == 1)
			break ;
		usleep(500);
	}
}
