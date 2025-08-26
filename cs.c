/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cs.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:49:05 by muokcan           #+#    #+#             */
/*   Updated: 2025/08/26 19:16:50 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void	cs_init(t_cs *cs, void *data)
{
	cs->data = data;
	cs->mutex = malloc(sizeof(pthread_mutex_t));
	if (cs->mutex == NULL)
	{
		exit(EXIT_FAILURE);
	}
	if (pthread_mutex_init(cs->mutex, NULL) != 0)
	{
		free(cs->mutex);
		exit(EXIT_FAILURE);
	}
}
