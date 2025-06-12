/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:15:08 by anebbou           #+#    #+#             */
/*   Updated: 2025/05/23 02:00:00 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*t_philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!philo->data->stop)
	{
		take_forks(philo);
		eat(philo);
		release_forks(philo);
		if (philo->data->stop)
			break ;
		sleep_think(philo);
	}
	return (NULL);
}
