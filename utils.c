/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shashemi <shashemi@student.42madrid.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-06-14 11:41:53 by shashemi          #+#    #+#             */
/*   Updated: 2025-06-14 11:41:53 by shashemi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

int	ft_print_and_check(t_philo *philo, char *status, int kill)
{
	pthread_mutex_lock(philo->death);
	if (*philo->check_dead)
	{
		pthread_mutex_unlock(philo->death);
		return (1);
	}
	if (status)
		printf ("%llu %d %s\n", (ft_get_time() - philo->start_time),
			philo->id + 1, status);
	if (kill)
		*philo->check_dead = 1;
	if (ft_get_time() - philo->last_meal > philo->time_to_die)
	{
		if (!*philo->check_dead)
		{
			printf ("%llu %d died\n", (ft_get_time() - philo->start_time),
				philo->id + 1);
			*philo->check_dead = 1;
		}
		pthread_mutex_unlock(philo->death);
		return (1);
	}
	pthread_mutex_unlock(philo->death);
	return (*philo->check_dead);
}

int	ft_philo_wait_time(t_philo *philo, t_time wait_time)
{
	t_time	time;

	time = ft_get_time();
	while (ft_get_time() - time < wait_time)
	{
		if (ft_print_and_check(philo, NULL, 0))
			return (1);
		usleep(10);
	}
	return (0);
}

static long	ft_atol(const char *str)
{
	int		i;
	long	res;
	int		sign;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] <= 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
		res = res * 10 + str[i++] - 48;
	return (res * sign);
}

int	ft_parse_args(int ac, char **av, long *values)
{
	int	i;
	int	j;

	if ((ac != 5 && ac != 6) || av[1][0] == '0')
		return (0);
	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
			if (!(av[i][j] >= '0' && av[i][j] <= '9'))
				return (0);
		values[i - 1] = ft_atol(av[i]);
	}
	if (ac == 5)
		values[4] = -1;
	return (1);
}

t_time	ft_get_time(void)
{
	struct timeval	tp;
	t_time			time;

	gettimeofday(&tp, NULL);
	time = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	return (time);
}
