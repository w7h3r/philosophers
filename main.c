/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:09:09 by muokcan           #+#    #+#             */
/*   Updated: 2025/08/25 16:01:40 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int	parse_arguments(int argc, char **argv, t_data *data)
{
	data->philosopher_num = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->philosopher_must_eat_num = ft_atoi(argv[5]);
	else
		data->philosopher_must_eat_num = -1;
	if (data->philosopher_num < 1 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0
		|| (argc == 6 && data->philosopher_must_eat_num < 0))
	{
		printf("Error: Invalid argument values\n");
		return (1);
	}
	return (0);
}

void	free_mutexes(t_data *data)
{
	if (data->forks)
	{
		for (int i = 0; i < data->philosopher_num; i++)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}

void	free_philos(t_philo *philos)
{
	t_philo	*temp;

	while (philos)
	{
		temp = philos;
		philos = philos->next;
		free(temp);
	}
}

void	free_all(t_data *data, t_philo *philos)
{
	free_mutexes(data);
	free_philos(philos);
}

int	init_philos(t_data *data, t_philo **philo)
{
	return (0);
}

static int	init(t_data *data, t_philo *philos, int argc, char **argv)
{
	if (parse_arguments(argc, argv, data) == 1)
		return (1);
	// Initialize mutexes, philosophers, and other necessary
	if (init_philos(data, &philos) == 1)
	{
		free_all(data, philos);
		return (1);
	}
	return (0);
}

long	get_time(void)
{
	
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philos;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid number of arguments\n");
		return (1);
	}
	if (init(&data, philos, argc, argv) == 1)
		return (1);
	start_simulation();
	free_all();
	return (0);
}
