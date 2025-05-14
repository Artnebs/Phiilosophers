/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:36:23 by anebbou           #+#    #+#             */
/*   Updated: 2025/05/12 13:52:59 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

long	get_time(void)
{
	struct timeval	tv;
	long			time_in_ms;

	gettimeofday(&tv, NULL);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}

void	print_status(t_data *data, int id, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&data->print_mutex);
	timestamp = get_time() - data->start_time;
	if (!data->stop)
		printf("%ld %d %s\n", timestamp, id, msg);
	pthread_mutex_unlock(&data->print_mutex);
}

static int	all_meals_eaten(t_data *data)
{
	int	j;

	j = 0;
	while (j < data->nb_philos)
	{
		if (data->philos[j].meals_eaten < data->nb_meals)
			return (0);
		j++;
	}
	return (1);
}

/* --- Helpers for init_data below --- */
static int	parse_arguments(t_data *data, int ac, char **av)
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
	return (0);
}

static int	allocate_resources(t_data *data)
{
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
		return (1);
	return (0);
}

static int	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->check_mutex, NULL) != 0)
		return (1);
	return (0);
}

int	init_data(t_data *data, int ac, char **av)
{
	int	ret;

	ret = parse_arguments(data, ac, av);
	if (ret)
		return (1);
	data->start_time = get_time();
	ret = allocate_resources(data);
	if (ret)
		return (1);
	ret = init_mutexes(data);
	if (ret)
		return (1);
	return (0);
}

static int	check_philosopher(t_data *data, int idx)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(&data->check_mutex);
	if ((get_time() - data->philos[idx].last_meal) > data->time_to_die)
	{
		print_status(data, data->philos[idx].id, "died");
		data->stop = 1;
		ret = 1;
	}
	else if (data->nb_meals != -1 && all_meals_eaten(data))
	{
		data->stop = 1;
		ret = 1;
	}
	pthread_mutex_unlock(&data->check_mutex);
	return (ret);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->nb_philos)
		{
			if (check_philosopher(data, i))
				return (NULL);
			usleep(1000);
			i++;
		}
	}
	return (NULL);
}

int	init_philos(t_data *data)
{
	int	i;

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

void	cleanup(t_data *data)
{
	int	i;

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
