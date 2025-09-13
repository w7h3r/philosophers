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
	long long	safety_time;

	think_time = data->time_to_die - (data->time_to_sleep + data->time_to_eat);
	if (think_time < 10)
		return (1);
	if (data->philosopher_num > 100)
		safety_time = think_time / 8;
	else
		safety_time = think_time / 4;
	return (think_time - safety_time);
}

void	my_sleep(long long time_in_ms, t_data *data)
{
	long long	current_time;
	long long	remaining_time;
	long long	estimated_end_time;

	estimated_end_time = get_time() + time_in_ms;
	while (1)
	{
		if (should_sim_stop(data) == 1)
			break ;
		current_time = get_time();
		if (current_time >= estimated_end_time)
			break ;
		remaining_time = estimated_end_time - current_time;
		usleep(100);
	}
}
