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
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

void	join_threads(t_data *data, t_philo *philos);
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

int	should_sim_stop(t_data *data);
long long	get_time(void);

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
	pthread_mutex_destroy(&data->last_meal_mutex);
	pthread_mutex_destroy(&data->times_ate_mutex);
	pthread_mutex_destroy(&data->all_ate_mutex);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	should_sim_stop(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->death_mutex);
	status = data->life;
	pthread_mutex_unlock(&data->death_mutex);
	return (!status);
}

void	my_sleep(long long time_in_ms)
{
	long long	current_time;
	long long	remaining_time;
	long long	estimated_end_time;

	estimated_end_time = get_time() + time_in_ms;
	while (1)
	{
		current_time = get_time();
		if (current_time >= estimated_end_time)
			break ;
		remaining_time = estimated_end_time - current_time;
		if (remaining_time > 5)
			usleep(1000);
		else
			usleep(100);
	}
}

int	philo_take_forks(t_philo *philo);
int	philo_put_down_forks(t_philo *philo);

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
	pthread_mutex_lock(&data->last_meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&data->last_meal_mutex);
	my_sleep(data->time_to_eat);
	pthread_mutex_lock(&data->times_ate_mutex);
	philo->times_ate++;
	curr_ate_num = philo->times_ate;
	pthread_mutex_unlock(&data->times_ate_mutex);
	if (data->philosopher_must_eat_num != -1 && curr_ate_num == data->philosopher_must_eat_num)
	{
		pthread_mutex_lock(&data->all_ate_mutex);
		data->all_ate++;
		pthread_mutex_unlock(&data->all_ate_mutex);
	}
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
	my_sleep(data->time_to_sleep);
	return (should_sim_stop(data));
}

int	philo_think(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (should_sim_stop(data) == 1)
		return (1);
	print_status(data, philo->id, "is thinking");
	my_sleep((data->time_to_die - (data->time_to_sleep + data->time_to_eat)) / 2);
	return (should_sim_stop(data));
}

int	philo_take_forks(t_philo *philo)
{
	t_data	*data;
	int		f_fork;
	int		s_fork;

	if (philo->id % 2 == 0)
	{
		f_fork = philo->right_fork;
		s_fork = philo->left_fork;
	}
	else
	{
		f_fork = philo->left_fork;
		s_fork = philo->right_fork;
	}
	data = philo->data;
	if (should_sim_stop(data) == 1)
		return (1);
	pthread_mutex_lock(&data->forks[f_fork]);
	print_status(data, philo->id, "has taken a fork");
	if (should_sim_stop(data) == 1)
	{
		pthread_mutex_unlock(&data->forks[f_fork]);
		return (1);
	}
	if (data->philosopher_num == 1)
	{
		pthread_mutex_unlock(&data->forks[f_fork]);
		return (1);
	}
	pthread_mutex_lock(&data->forks[s_fork]);
	print_status(data, philo->id, "has taken a fork");
	if (should_sim_stop(data) == 1)
	{
		pthread_mutex_unlock(&data->forks[f_fork]);
		pthread_mutex_unlock(&data->forks[s_fork]);
		return (1);
	}
	return (0);
}

int	philo_put_down_forks(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
		pthread_mutex_unlock(&data->forks[philo->right_fork]);
	}
	else
	{
		pthread_mutex_unlock(&data->forks[philo->right_fork]);
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
	}
	return (0);
}

int	get_life_status(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->death_mutex);
	status = data->life;
	pthread_mutex_unlock(&data->death_mutex);
	return (status);
}

void	*philo_routine(void *arg)
{
	t_data	*data;
	t_philo	*philo;
	int		curr_ate_num;

	philo = (t_philo *)arg;
	data = philo->data;
	while (get_life_status(data) == 0)
		;
	pthread_mutex_lock(&data->last_meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&data->last_meal_mutex);
	if (philo->id % 2 == 0)
		my_sleep(data->time_to_eat / 10);
	if (philo->id == data->philosopher_num && data->philosopher_num % 2 == 1 && data->philosopher_num != 1)
		my_sleep(data->time_to_eat + data->time_to_sleep / 2);
	while (should_sim_stop(data) == 0)
	{
		if (philo_eat(philo) == 1)
			break ;
		if (data->philosopher_must_eat_num != -1)
		{
			pthread_mutex_lock(&data->times_ate_mutex);
			curr_ate_num = philo->times_ate;
			pthread_mutex_unlock(&data->times_ate_mutex);
			if (curr_ate_num >= data->philosopher_must_eat_num)
				break ;
		}
		if (philo_sleep(philo) == 1)
			break ;
		if (philo_think(philo) == 1)
			break ;
	}
	return ((void *)0);
}

void	free_all(t_data *data, t_philo *philos)
{
	if (data)
		free_mutexes(data);
	if (philos)
	{
		free(philos);
	}
}

int	init_philos(t_data *data, t_philo **philo)
{
	int	i;

	*philo = malloc(sizeof(t_philo) * data->philosopher_num);
	if (!*philo)
		return (1);
	data->philos = *philo;
	i = 0;
	while (i < data->philosopher_num)
	{
		(*philo)[i].id = i + 1;
		(*philo)[i].times_ate = 0;
		(*philo)[i].left_fork = i;
		(*philo)[i].right_fork = (i + 1) % data->philosopher_num;
		(*philo)[i].data = data;
		(*philo)[i].last_meal_time = get_time();
		if (pthread_create(&(*philo)[i].thread, NULL, (void *(*)(void *))philo_routine, &(*philo)[i]) != 0)
		{
			pthread_mutex_lock(&data->death_mutex);
			data->life = 0;
			pthread_mutex_unlock(&data->death_mutex);
			while (i-- > 0)
				pthread_join((*philo)[i].thread, NULL);
			free_all(data, *philo);
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
	data->life = 0;
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		free_all(data, NULL);
		return (1);
	}
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
	{
		free_all(data, NULL);
		return (1);
	}
	if (pthread_mutex_init(&data->last_meal_mutex, NULL) != 0)
	{
		free_all(data, NULL);
		return (1);
	}
	if (pthread_mutex_init(&data->times_ate_mutex, NULL) != 0)
	{
		free_all(data, NULL);
		return (1);
	}
	if (pthread_mutex_init(&data->all_ate_mutex, NULL) != 0)
	{
		free_all(data, NULL);
		return (1);
	}
	if (parse_arguments(argc, argv, data) == 1)
	{
		free_all(data, NULL);
		return (1);
	}
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
	data->all_ate = 0;
	data->start_time = get_time();
	pthread_mutex_lock(&data->death_mutex);
	data->life = 1;
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}

int	has_everyone_eaten(t_data *data)
{
	int	all_ate;

	if (data->philosopher_must_eat_num == -1)
		return (0);
	pthread_mutex_lock(&data->all_ate_mutex);
	if (data->all_ate >= data->philosopher_num)
		all_ate = 1;
	else
		all_ate = 0;
	pthread_mutex_unlock(&data->all_ate_mutex);
	return (all_ate);
}

long long	get_philo_last_meal_time(t_philo *philo)
{
	long long	last_meal_time;

	pthread_mutex_lock(&philo->data->last_meal_mutex);
	last_meal_time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->data->last_meal_mutex);
	return (last_meal_time);
}

void	philo_monitor(t_data *data, t_philo *philos)
{
	int	i;
	long long	current_time;
	long long	last_meal_time;
	long long	time_diff;

		while (should_sim_stop(data) == 0)
		{
			i = 0;
			while (i < data->philosopher_num)
			{
				current_time = get_time();
				last_meal_time = get_philo_last_meal_time(&philos[i]);
				time_diff = current_time - last_meal_time;
				if (time_diff >= data->time_to_die)
				{
					pthread_mutex_lock(&data->print_mutex);
					pthread_mutex_lock(&data->death_mutex);
					data->life = 0;
					pthread_mutex_unlock(&data->death_mutex);
					printf("%lld %d died\n", get_time() - data->start_time, philos[i].id);
					pthread_mutex_unlock(&data->print_mutex);
					return ;
				}
				i++;
			}
			if (has_everyone_eaten(data) == 1)
			{
				pthread_mutex_lock(&data->death_mutex);
				data->life = 0;
				pthread_mutex_unlock(&data->death_mutex);
				return ;
			}
			usleep(500);
		}
}

void	join_threads(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->philosopher_num)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philos;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid number/pattern of arguments\nUsage: ./philo philo_number time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	if (init(&data, &philos, argc, argv) == 1)
		return (1);
	philo_monitor(&data, philos);
	join_threads(&data, philos);
	free_all(&data, philos);
	return (0);
}
