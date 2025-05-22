/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:19:43 by anebbou           #+#    #+#             */
/*   Updated: 2025/05/22 21:27:58 by anebbou          ###   ########.fr       */
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

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!data->stop)
	{
		pthread_mutex_lock(&data->check_mutex);
		i = 0;
		while (i < data->nb_philos && !data->stop)
		{
			if (get_time() - data->philos[i].last_meal > data->time_to_die)
			{
				print_status(data, data->philos[i].id, "died");
				data->stop = 1;
			}
			i++;
		}
		if (!data->stop && data->nb_meals != -1 && all_meals_eaten(data))
			data->stop = 1;
		pthread_mutex_unlock(&data->check_mutex);
		usleep(1000);
	}
	return (NULL);
}
