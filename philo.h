/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:09:15 by muokcan           #+#    #+#             */
/*   Updated: 2025/08/26 18:35:37 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <complex.h>
# include <pthread.h>

typedef struct s_data
{
	int				sim_start;
	int				philosopher_num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				philosopher_must_eat_num;
	long long		start_time;
	int				all_ate;
	int				someone_died;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	long long		last_meal_time;
	int				times_ate;
	t_data			*data;
	pthread_t		thread;
	struct s_philo	*next;
}	t_philo;

typedef struct s_cs
{
	void			*data;
	pthread_mutex_t	*mutex;
}	t_cs;

int	ft_atoi(const char *nptr);
void	free_all(t_data *data, t_philo *philos);
void	free_philos(t_philo *philos);

#endif
