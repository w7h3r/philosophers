/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 17:12:47 by muokcan           #+#    #+#             */
/*   Updated: 2025/09/13 17:29:27 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int	isnan(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

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
		printf(VALUE_ERROR);
		return (1);
	}
	if (isnan(argv[1]) || isnan(argv[2]) || isnan(argv[3])
		|| isnan(argv[4]) || (argc == 6 && isnan(argv[5])))
	{
		printf(VALUE_ERROR);
		return (1);
	}
	data->time_to_think = set_think_time(data);
	return (0);
}
