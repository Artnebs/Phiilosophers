/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:36:23 by anebbou           #+#    #+#             */
/*   Updated: 2025/03/27 15:41:20 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** get_time:
** - Uses gettimeofday to get the current time.
** - Converts the time to milliseconds.
** - This is used for timestamps when logging philosopher events.
*/
long get_time(void)
{
	struct timeval tv;
	long time_in_ms;

	gettimeofday(&tv, NULL);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}

/*
** print_status:
** - Locks the print mutex to ensure only one thread prints at a time.
** - Calculates the timestamp (current time - start time) and prints the message.
** - Unlocks the mutex after printing.
*/
void print_status(t_data *data, int id, char *msg)
{
	long timestamp;

	pthread_mutex_lock(&data->print_mutex);
	timestamp = get_time() - data->start_time;
	if (!data->stop)
		printf("%ld %d %s\n", timestamp, id, msg);
	pthread_mutex_unlock(&data->print_mutex);
}

/*
** monitor_routine:
** - Runs in a separate thread to check all philosophers.
** - If a philosopher hasn’t eaten within time_to_die, prints that the philosopher died
**   and stops the simulation.
** - Also checks if all philosophers have eaten the required number of meals (if specified).
*/
void *monitor_routine(void *arg)
{
	t_data *data;
	int i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->nb_philos)
		{
			pthread_mutex_lock(&data->check_mutex);
			if ((get_time() - data->philos[i].last_meal) > data->time_to_die)
			{
				print_status(data, data->philos[i].id, "died");
				data->stop = 1;
				pthread_mutex_unlock(&data->check_mutex);
				return (NULL);
			}
			// Check if the optional meal count has been reached for all philosophers.
			if (data->nb_meals != -1)
			{
				int done = 1;
				for (int j = 0; j < data->nb_philos; j++)
				{
					if (data->philos[j].meals_eaten < data->nb_meals)
					{
						done = 0;
						break;
					}
				}
				if (done)
				{
					data->stop = 1;
					pthread_mutex_unlock(&data->check_mutex);
					return (NULL);
				}
			}
			pthread_mutex_unlock(&data->check_mutex);
			usleep(1000);
			i++;
		}
	}
	return (NULL);
}

/*
** init_data:
** - Reads command-line arguments to set the simulation parameters.
** - Allocates memory for the philosopher array and the fork mutex array.
** - Initializes all fork mutexes and the print and check mutexes.
*/
int init_data(t_data *data, int ac, char **av)
{
	data->nb_philos = atoi(av[1]);
	data->time_to_die = atol(av[2]);
	data->time_to_eat = atol(av[3]);
	data->time_to_sleep = atol(av[4]);
	if (ac == 6)
		data->nb_meals = atoi(av[5]);
	else
		data->nb_meals = -1;
	data->stop = 0;
	data->start_time = get_time();
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
		return (1);
	for (int i = 0; i < data->nb_philos; i++)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->check_mutex, NULL) != 0)
		return (1);
	return (0);
}

/*
** init_philos:
** - Initializes each philosopher:
**   - Sets the philosopher's ID.
**   - Sets the initial number of meals eaten and the time of the last meal.
**   - Assigns the correct left and right forks (the right fork wraps around for the last philosopher).
*/
int init_philos(t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal = data->start_time;
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philos];
		i++;
	}
	return (0);
}

/*
** cleanup:
** - Destroys all fork mutexes and the print and check mutexes.
** - Frees the allocated memory for the forks and philosophers.
*/
void cleanup(t_data *data)
{
	int i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->check_mutex);
	free(data->forks);
	free(data->philos);
}
