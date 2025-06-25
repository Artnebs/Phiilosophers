/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 10:18:50 by anebbou           #+#    #+#             */
/*   Updated: 2025/05/22 21:36:44 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	lock_fork(pthread_mutex_t *fork, t_data *data)
{
	while (!atomic_load(&data->stop))
	{
		if (pthread_mutex_trylock(fork) == 0)
			return (1);
		usleep(100);
	}
	return (0);
}

int	take_forks(t_philo *philo)
{
	if (philo->data->nb_philos == 1)
	{
		if (!lock_fork(philo->left_fork, philo->data))
			return (0);
		return (print_status(philo->data, philo->id, "has taken a fork"), 1);
	}
	if (philo->id % 2 == 0)
	{
		if (!lock_fork(philo->right_fork, philo->data))
			return (0);
		print_status(philo->data, philo->id, "has taken a fork");
		if (!lock_fork(philo->left_fork, philo->data))
			return (pthread_mutex_unlock(philo->right_fork), 0);
		print_status(philo->data, philo->id, "has taken a fork");
	}
	else
	{
		if (!lock_fork(philo->left_fork, philo->data))
			return (0);
		print_status(philo->data, philo->id, "has taken a fork");
		if (!lock_fork(philo->right_fork, philo->data))
			return (pthread_mutex_unlock(philo->left_fork), 0);
		print_status(philo->data, philo->id, "has taken a fork");
	}
	return (1);
}

void	release_forks(t_philo *philo)
{
	if (philo->data->nb_philos == 1)
	{
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->check_mutex);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->check_mutex);
	print_status(philo->data, philo->id, "is eating");
	safe_sleep(philo->data->time_to_eat, philo->data);
}

void	sleep_think(t_philo *philo)
{
	print_status(philo->data, philo->id, "is sleeping");
	safe_sleep(philo->data->time_to_sleep, philo->data);
	print_status(philo->data, philo->id, "is thinking");
}
