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
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

int	parse_arguments(int argc, char **argv, t_data *data)
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
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->philosopher_num)
			pthread_mutex_destroy(&data->forks[i++]);
		free(data->forks);
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	*philo_routine(void *arg)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo *)arg;
	data = philo->data;
	while (data->life == 0)
		usleep(100);
	pthread_mutex_lock(&data->death_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&data->death_mutex);
	while (data->life)
	{
		if (philo->right_fork < philo->left_fork)
		{
			pthread_mutex_lock(&data->forks[philo->right_fork]);
			pthread_mutex_lock(&data->forks[philo->left_fork]);
		}
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		pthread_mutex_lock(&data->forks[philo->right_fork]);
		printf("Philosopher %d is eating\n", philo->id);
		philo->last_meal_time = get_time();
		philo->times_ate++;
		usleep(data->time_to_eat * 1000);
		printf("Philosopher %d has eaten | %lld\n",philo->id, get_time() - data->start_time);
		// Put down forks
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
		pthread_mutex_unlock(&data->forks[philo->right_fork]);

		// Sleeping
		printf("Philosopher %d is sleeping\n", philo->id);
		usleep(data->time_to_sleep * 1000);

		// Thinking
		printf("Philosopher %d is thinking\n", philo->id);
		usleep((data->time_to_eat + data->time_to_sleep - data->time_to_die) * 1000);
	}
	return (0);
}

void	free_philos(t_philo *philos)
{
	if (philos)
		free(philos);
}

void	free_all(t_data *data, t_philo *philos)
{
	if (data)
		free_mutexes(data);
	if (philos)
		free_philos(philos);
}

int	init_philos(t_data *data, t_philo **philo)
{
	int	i;

	*philo = malloc(sizeof(t_philo) * data->philosopher_num);
	if (!*philo)
		return (1);
	i = 0;
	while (i < data->philosopher_num)
	{
		(*philo)[i].id = i + 1;
		(*philo)[i].times_ate = 0;
		(*philo)[i].left_fork = i;
		(*philo)[i].right_fork = (i + 1) % data->philosopher_num;
		(*philo)[i].data = data;
		(*philo)[i].last_meal_time = 0;
		if (pthread_create(&(*philo)[i].thread, NULL, (void *(*)(void *))philo_routine, &(*philo)[i]) != 0)
		{
			free_philos(*philo);
			return (1);
		}
		i++;
	}
	return (0);
}

void	destroy_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philosopher_num)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
}

int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->philosopher_num);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->philosopher_num)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			destroy_forks(data);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	init(t_data *data, t_philo **philos, int argc, char **argv)
{
	if (parse_arguments(argc, argv, data) == 1)
		return (1);
	data->all_ate = 0;
	data->start_time = get_time();
	if (init_forks(data) == 1)
	{
		free_all(data, NULL);
		return (1);
	}
	if (init_philos(data, philos) == 1)
	{
		free_all(data, *philos);
		return (1);
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		free_all(data, *philos);
		return (1);
	}
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
	{
		free_all(data, *philos);
		return (1);
	}
	data->life = 1;
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philos;
	int		i;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid number/pattern of arguments\nUsage: ./philo philo_number time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	if (init(&data, &philos, argc, argv) == 1)
		return (1);
	while (data.life == 1)
	{
		i = 0;
		while (i < data.philosopher_num)
		{
			pthread_mutex_lock(&data.death_mutex);
			if (get_time() - philos->last_meal_time > data.time_to_die)
			{
				pthread_mutex_lock(&data.print_mutex);
				printf("&lt;timestamp&gt; %lld &lt;philosopher_id&gt; %d &lt;action&gt; died\n", get_time() - data.start_time, philos[i].id);
				data.life = 0;
				pthread_mutex_unlock(&data.print_mutex);
				pthread_mutex_unlock(&data.death_mutex);
				break ;
			}
			pthread_mutex_unlock(&data.death_mutex);
			i++;
		}
		usleep(1000);
	}
	i = 0;
	while (i < data.philosopher_num)
		pthread_join(philos[i++].thread, NULL);
	free_all(&data, philos);
	return (0);
}
