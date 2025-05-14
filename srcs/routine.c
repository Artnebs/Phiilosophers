/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:19:20 by anebbou           #+#    #+#             */
/*   Updated: 2025/05/14 18:17:01 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*t_philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		if (philo->data->stop)
			break ;
		take_forks(philo);
		if (philo->left_fork == philo->right_fork)
			break ;
		eat(philo);
		release_forks(philo);
		print_status(philo->data, philo->id, "is sleeping");
		usleep(philo->data->time_to_sleep * 1000);
		print_status(philo->data, philo->id, "is thinking");
	}
	return (NULL);
}
