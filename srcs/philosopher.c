/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:36:13 by anebbou           #+#    #+#             */
/*   Updated: 2025/03/27 15:41:01 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** take_forks:
** - Locks (picks up) the left fork and then the right fork.
** - Prints a message each time a fork is taken.
** - If there is only one fork (single philosopher), it waits to simulate that the philosopher
**   cannot eat and then unlocks the fork.
*/
void take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo->data, philo->id, "has taken a fork");
	if (philo->left_fork == philo->right_fork)
	{
		usleep(philo->data->time_to_die * 1000);
		pthread_mutex_unlock(philo->left_fork);
		return;
	}
	pthread_mutex_lock(philo->right_fork);
	print_status(philo->data, philo->id, "has taken a fork");
}

/*
** release_forks:
** - Unlocks (puts down) both forks.
*/
void release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

/*
** eat:
** - Updates the last meal time to the current time.
** - Prints that the philosopher is eating.
** - Sleeps for the time required to eat.
** - Increases the meals_eaten counter.
*/
void eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->check_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->check_mutex);
	print_status(philo->data, philo->id, "is eating");
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_lock(&philo->data->check_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->check_mutex);
}

/*
** t_philosopher:
** - This is the main function run by each philosopher thread.
** - Even numbered philosophers wait a little bit at the beginning to help prevent deadlock.
** - The loop makes the philosopher take forks, eat, release forks, sleep, and think repeatedly.
** - The loop breaks if the simulation is stopped (either by a death or when all meals are completed).
*/
void *t_philosopher(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		if (philo->data->stop)
			break;
		take_forks(philo);
		// In the single philosopher case, the philosopher cannot eat.
		if (philo->left_fork == philo->right_fork)
			break;
		eat(philo);
		release_forks(philo);
		print_status(philo->data, philo->id, "is sleeping");
		usleep(philo->data->time_to_sleep * 1000);
		print_status(philo->data, philo->id, "is thinking");
	}
	return (NULL);
}
