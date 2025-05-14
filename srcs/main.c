/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:32:25 by anebbou           #+#    #+#             */
/*   Updated: 2025/05/12 13:35:12 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	launch_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_create(
				&data->philos[i].thread,
				NULL,
				t_philosopher,
				&data->philos[i]
			) != 0)
			return (1);
		i++;
	}
	return (0);
}

static void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_data		data;
	pthread_t	monitor;

	if (ac != 5 && ac != 6)
	{
		printf("Usage: %s number_of_philos time_to_die time_to_eat "
			"time_to_sleep [nb_meals]\n", av[0]);
		return (1);
	}
	if (init_data(&data, ac, av) || init_philos(&data))
		return (1);
	if (launch_threads(&data))
		return (1);
	if (pthread_create(
			&monitor,
			NULL,
			monitor_routine,
			&data
		) != 0)
		return (1);
	join_threads(&data);
	pthread_join(monitor, NULL);
	cleanup(&data);
	return (0);
}
