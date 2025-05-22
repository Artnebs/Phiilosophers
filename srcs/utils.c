/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:36:23 by anebbou           #+#    #+#             */
/*   Updated: 2025/05/22 20:15:04 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	if (!data->stop || (msg && !ft_strcmp(msg, "died")))
		printf("%ld %d %s\n", timestamp, id, msg);
	pthread_mutex_unlock(&data->print_mutex);
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

void	safe_sleep(long time_in_ms, t_data *data)
{
	long	start;

	start = get_time();
	while (!data->stop && (get_time() - start) < time_in_ms)
		usleep(500);
}
