/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:19:43 by anebbou           #+#    #+#             */
/*   Updated: 2025/05/14 18:16:39 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
