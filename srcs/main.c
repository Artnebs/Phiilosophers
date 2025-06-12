/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:32:25 by anebbou           #+#    #+#             */
/*   Updated: 2025/05/22 20:49:08 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	handle_thread_error(t_data *data, int i)
{
	data->stop = 1;
	while (--i >= 0)
		pthread_join(data->philos[i].thread, NULL);
	return (1);
}

static int	launch_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				t_philosopher, &data->philos[i]) != 0)
			return (handle_thread_error(data, i));
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

static int	init_and_launch(t_data *data, int ac, char **av)
{
	if (init_data(data, ac, av) || init_philos(data))
		return (1);
	if (launch_threads(data))
	{
		cleanup(data);
		return (1);
	}
	return (0);
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
	if (init_and_launch(&data, ac, av))
		return (1);
	usleep(200);
	if (pthread_create(&monitor, NULL, monitor_routine, &data) != 0)
	{
		data.stop = 1;
		join_threads(&data);
		cleanup(&data);
		return (1);
	}
	join_threads(&data);
	pthread_join(monitor, NULL);
	cleanup(&data);
	return (0);
}
