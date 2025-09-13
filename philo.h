/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:09:15 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/13 17:31:35 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

typedef struct s_data
{
	int				life;
	int				philosopher_num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_think;
	int				philosopher_must_eat_num;
	long long		start_time;
	int				all_ate;
	struct s_philo	*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	times_ate_mutex;
	pthread_mutex_t	all_ate_mutex;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	long long		last_meal_time;
	pthread_mutex_t	last_meal_mutex;
	int				times_ate;
	t_data			*data;
	pthread_t		thread;
	struct s_philo	*next;
}	t_philo;

int			ft_atoi(const char *nptr);
void		free_all(t_data *data, t_philo *philos);
void		join_threads(t_data *data, t_philo *philos);
void		philo_monitor(t_data *data, t_philo *philos);
long long	get_philo_last_meal_time(t_philo *philo);
int			has_everyone_eaten(t_data *data);
int			init(t_data *data, t_philo **philos, int argc, char **argv);
int			init_forks(t_data *data);
void		destroy_forks(t_data *data);
int			init_philos(t_data *data, t_philo **philo);
void		free_all(t_data *data, t_philo *philos);
void		*philo_routine(void *arg);
int			get_life_status(t_data *data);
int			philo_put_down_forks(t_philo *philo);
int			philo_take_forks(t_philo *philo);
int			philo_think(t_philo *philo);
long long	set_think_time(t_data *data);
int			philo_sleep(t_philo *philo);
int			philo_eat(t_philo *philo);
void		my_sleep(long long time_in_ms, t_data *data);
int			should_sim_stop(t_data *data);
long long	get_time(void);
void		free_mutexes(t_data *data);
void		print_status(t_data *data, int id, char *status);
int			parse_arguments(int argc, char **argv, t_data *data);
int			free_all_and_return(t_data *data, t_philo *philos, int code);
int			get_first_fork_id(t_philo *philo);
int			get_second_fork_id(t_philo *philo);

#endif
