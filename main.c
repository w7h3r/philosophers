/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muokcan <muokcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 17:09:09 by muokcan           #+#    #+#             */
/*   Updated: 2025/08/24 17:24:43 by muokcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>


int	main(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid number of arguments\n");
		return (1);
	}
	if (init_all(argc, argv) == 1)
		return (1);
	start_simulation();
	clean_all();
	return (0);
}
