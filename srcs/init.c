/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:15:08 by anebbou           #+#    #+#             */
/*   Updated: 2025/05/22 20:54:54 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	parse_arguments(t_data *data, int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (!is_number(av[i]))
			return (1);
		i++;
	}
	data->nb_philos = ft_atol_safe(av[1]);
	data->time_to_die = ft_atol_safe(av[2]);
	data->time_to_eat = ft_atol_safe(av[3]);
	data->time_to_sleep = ft_atol_safe(av[4]);
	if (ac == 6)
		data->nb_meals = ft_atol_safe(av[5]);
	else
		data->nb_meals = -1;
	if (data->nb_philos <= 0 || data->nb_philos > INT_MAX
		|| data->time_to_die <= 0 || data->time_to_die > INT_MAX
		|| data->time_to_eat <= 0 || data->time_to_eat > INT_MAX
		|| data->time_to_sleep <= 0 || data->time_to_sleep > INT_MAX
		|| (ac == 6 && (data->nb_meals <= 0 || data->nb_meals > INT_MAX)))
		return (1);
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
	{
		free(data->philos);
		return (1);
	}
	return (0);
}

static int	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			free(data->forks);
			free(data->philos);
			return (1);
		}
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0
		|| pthread_mutex_init(&data->check_mutex, NULL) != 0)
	{
		while (--i >= 0)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
		free(data->philos);
		return (1);
	}
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
