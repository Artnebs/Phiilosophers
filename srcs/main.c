/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:32:25 by anebbou           #+#    #+#             */
/*   Updated: 2025/03/27 15:40:41 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** main:
** - Checks the number of arguments.
** - Initializes the simulation data and philosophers.
** - Creates one thread per philosopher (each runs t_philosopher).
** - Creates a monitor thread that checks if any philosopher dies.
** - Waits for all threads to finish and cleans up resources.
*/
int main(int ac, char **av)
{
	t_data data;
	int i;
	pthread_t monitor;

	if (ac != 5 && ac != 6)
	{
		printf("Usage: %s number_of_philos time_to_die time_to_eat time_to_sleep [nb_meals]\n", av[0]);
		return (1);
	}
	if (init_data(&data, ac, av))
		return (1);
	if (init_philos(&data))
		return (1);
	// Create philosopher threads.
	i = 0;
	while (i < data.nb_philos)
	{
		if (pthread_create(&data.philos[i].thread, NULL, t_philosopher, &data.philos[i]) != 0)
			return (1);
		i++;
	}
	// Create the monitor thread to watch for deaths.
	if (pthread_create(&monitor, NULL, monitor_routine, &data) != 0)
		return (1);
	// Wait for all philosopher threads to finish.
	i = 0;
	while (i < data.nb_philos)
	{
		pthread_join(data.philos[i].thread, NULL);
		i++;
	}
	// Wait for the monitor thread.
	pthread_join(monitor, NULL);
	cleanup(&data);
	return (0);
}
