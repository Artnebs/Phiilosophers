/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:19:20 by anebbou           #+#    #+#             */
/*   Updated: 2025/05/22 21:22:18 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*t_philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		safe_sleep(50, philo->data);
	while (!atomic_load(&philo->data->stop))
	{
		if (!take_forks(philo))
			break ;
		if (philo->data->nb_philos == 1)
			break ;
		if (atomic_load(&philo->data->stop))
		{
			release_forks(philo);
			break ;
		}
		eat(philo);
		release_forks(philo);
		print_status(philo->data, philo->id, "is sleeping");
		safe_sleep(philo->data->time_to_sleep, philo->data);
		print_status(philo->data, philo->id, "is thinking");
	}
	if (philo->data->nb_philos > 1)
		release_forks(philo);
	return (NULL);
}
