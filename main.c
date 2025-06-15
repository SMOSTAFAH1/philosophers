/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-14 11:33:30 by shashemi          #+#    #+#             */
/*   Updated: 2025-06-14 11:33:30 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	ft_init_data(t_philo *philo, long *values, pthread_mutex_t *forks,
		pthread_mutex_t *death)
{
	int	*check_dead;
	int	i;

	i = 0;
	check_dead = malloc(sizeof(int));
	*check_dead = 0;
	while (i < values[0])
	{
		philo[i].id = i;
		philo[i].number_of_philo = values[0];
		philo[i].time_to_die = values[1];
		philo[i].time_to_eat = values[2];
		philo[i].time_to_sleep = values[3];
		philo[i].must_eat = values[4];
		philo[i].last_meal = ft_get_time();
		philo[i].start_time = ft_get_time();
		philo[i].total_eaten = 0;
		philo[i].check_dead = check_dead;
		philo[i].left_fork = &forks[i];
		philo[i].right_fork = &forks[(i + 1) % values[0]];
		philo[i].death = death;
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	pthread_mutex_init(death, NULL);
}

void	ft_philo_eat_sleep_think(t_philo *philo)
{
	if (ft_print_and_check(philo, NULL, 0))
		return ;
	pthread_mutex_lock(philo->left_fork);
	ft_print_and_check(philo, "has taken a fork", 0);
	pthread_mutex_lock(philo->right_fork);
	if (ft_print_and_check(philo, NULL, 0))
		return ;
	ft_print_and_check(philo, "has taken a fork ", 0);
	pthread_mutex_lock(philo->death);
	philo->total_eaten++;
	pthread_mutex_unlock(philo->death);
	ft_print_and_check(philo, "is eating", 0);
	ft_philo_wait_time(philo, philo->time_to_eat);
	pthread_mutex_lock(philo->death);
	philo->last_meal = ft_get_time();
	pthread_mutex_unlock(philo->death);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	if (ft_print_and_check(philo, NULL, 0))
		return ;
	ft_print_and_check(philo, "is sleeping", 0);
	ft_philo_wait_time(philo, philo->time_to_sleep);
	if (ft_print_and_check(philo, NULL, 0))
		return ;
	ft_print_and_check(philo, "is thinking", 0);
}

void	*ft_philo_check(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	if (philo->number_of_philo == 1)
	{
		if (pthread_mutex_lock(philo->left_fork))
			return (NULL);
		ft_print_and_check(philo, " has taken a fork", 0);
		while (!ft_print_and_check(philo, NULL, 0))
			;
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		if (philo->must_eat == 0)
			return (NULL);
		while (1)
		{
			if (philo->total_eaten == philo->must_eat
				|| ft_print_and_check(philo, NULL, 0))
				return (NULL);
			ft_philo_eat_sleep_think(philo);
		}
	}
	return (NULL);
}

void	ft_create_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->number_of_philo)
	{
		pthread_create(&philo[i].thread, NULL, &ft_philo_check, &philo[i]);
		i++;
		usleep(100);
	}
	if (ft_print_and_check(philo, NULL, 0))
		return ;
	i = 0;
	while (i < philo->number_of_philo)
		pthread_join(philo[i++].thread, NULL);
}

int	main(int ac, char **av)
{
	t_philo			*philo;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*death;
	long			values[5];
	int				i;

	if (!ft_parse_args(ac, av, values))
		return (1);
	philo = malloc(sizeof(t_philo) * values[0]);
	forks = malloc(sizeof(pthread_mutex_t) * values[0]);
	death = malloc(sizeof(pthread_mutex_t));
	if (!philo || !forks || !death)
		return (1);
	ft_init_data(philo, values, forks, death);
	ft_create_threads(philo);
	printf("\n%d", philo->total_eaten);
	i = 0;
	while (i < philo->number_of_philo)
		pthread_mutex_destroy(&forks[i++]);
	pthread_mutex_destroy(death);
	free(philo->check_dead);
	free(forks);
	free(death);
	free(philo);
	return (0);
}
