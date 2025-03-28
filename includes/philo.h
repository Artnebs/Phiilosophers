/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anebbou <anebbou@student42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 15:32:04 by anebbou           #+#    #+#             */
/*   Updated: 2025/03/27 15:45:02 by anebbou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include "../Libft_GNL_Printf/includes/libft.h"
# include "../Libft_GNL_Printf/includes/ft_printf.h"
# include "../Libft_GNL_Printf/includes/get_next_line.h"

/*
** t_philo:
** - id: The philosopher's number.
** - last_meal: The last time (in ms) the philosopher started eating.
** - meals_eaten: How many times the philosopher has eaten.
** - thread: The thread representing this philosopher.
** - left_fork/right_fork: Pointers to the two mutexes (forks) this philosopher uses.
** - data: Pointer to the shared simulation data.
*/
typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	long			last_meal;
	int				meals_eaten;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}				t_philo;

/*
** t_data:
** - nb_philos: Number of philosophers (and forks).
** - time_to_die: Max time (in ms) a philosopher can go without eating.
** - time_to_eat: Time (in ms) a philosopher takes to eat.
** - time_to_sleep: Time (in ms) a philosopher sleeps.
** - nb_meals: (Optional) How many meals each philosopher must eat.
** - start_time: Timestamp when simulation started.
** - stop: Flag to stop the simulation.
** - philos: Array of philosopher structures.
** - forks: Array of mutexes representing forks.
** - print_mutex: Mutex used to protect printing.
** - check_mutex: Mutex used to protect checking of philosophers' state.
*/
struct s_data
{
	int				nb_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				nb_meals;
	long			start_time;
	int				stop;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	check_mutex;
};

long	get_time(void);
void	print_status(t_data *data, int id, char *msg);
void	*t_philosopher(void *arg);
void	*monitor_routine(void *arg);
int		init_data(t_data *data, int ac, char **av);
int		init_philos(t_data *data);
void	cleanup(t_data *data);

#endif
